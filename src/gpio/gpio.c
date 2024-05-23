#include <esp_log.h>
#include <mcp23x17.h>
#include <driver/gpio.h>
#include "configuration.h"
#include "gpio/globals.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#define BIT_BUTTON_CHANGED BIT(0)

static const char *TAG = "Button-Minigame-gpio";
static EventGroupHandle_t eg = NULL;
uint16_t last_io_expander_states[3];


static void IRAM_ATTR intr_handler(void *arg)
{
    BaseType_t hp_task;
    if (xEventGroupSetBitsFromISR(eg, BIT_BUTTON_CHANGED, &hp_task) != pdFAIL)
        portYIELD_FROM_ISR(hp_task);
}

static int get_changed_bit_position(uint16_t current, uint16_t previous) {
    uint16_t changed = current ^ previous;  // XOR to find changed bits
    
    for (int i = 0; i < 16; i++) {
        if (changed & (1 << i)) {
            return i;
        }
    }
    return -1;
}

static void button_handler(void *pvParameters)
{
    uint16_t io_expander_states;

    for (;;) {
        if (xEventGroupWaitBits(eg, BIT_BUTTON_CHANGED, pdTRUE, pdTRUE, portMAX_DELAY) != BIT_BUTTON_CHANGED)
            continue;      

        for (uint8_t i = 0; i < 3; i++) {
            mcp23x17_get_int_pin_states(io_expander_devs[i], &io_expander_states);
            // check if states differ from the previous states
            int changed_bit_position = get_changed_bit_position(io_expander_states, last_io_expander_states[i]);
            if (changed_bit_position == -1)  // no change
                continue;

            // get corresponding button
            for (int j = 0; j < sizeof(IOExpanderInputs)/sizeof(IOExpanderInputs[0]); j++) {
                if (IOExpanderInputs[j].io_expander_dev == io_expander_devs[i] && IOExpanderInputs[j].io_port == changed_bit_position) {
                    // TODO -> send to button logic + "debounce"
                    printf("%s\n", IOExpanderInputs[j].name);
                    break;
                }
            }

            last_io_expander_states[i] = io_expander_states;
            break;
        }
    }
}

static void init_io_expanders() {
    ESP_ERROR_CHECK(i2cdev_init());
    ESP_ERROR_CHECK(mcp23x17_init_desc(&io_expander_dev1, IO_EXPANDER1_ADDR, 0, IO_EXPANDER_SDA_PIN, IO_EXPANDER_SCL_PIN));  //left
    io_expander_dev1.cfg.master.clk_speed = 400 * 1000;
    ESP_ERROR_CHECK(mcp23x17_init_desc(&io_expander_dev2, IO_EXPANDER2_ADDR, 0, IO_EXPANDER_SDA_PIN, IO_EXPANDER_SCL_PIN));  // middle
    io_expander_dev2.cfg.master.clk_speed = 400 * 1000;
    ESP_ERROR_CHECK(mcp23x17_init_desc(&io_expander_dev3, IO_EXPANDER3_ADDR, 0, IO_EXPANDER_SDA_PIN, IO_EXPANDER_SCL_PIN));  // right
    io_expander_dev3.cfg.master.clk_speed = 400 * 1000;
    eg = xEventGroupCreate();

    // set initial io expander input states (pull up resistor, isr, etc.)
    for (int i = 0; i < sizeof(IOExpanderInputs)/sizeof(IOExpanderInputs[0]); i++) {
        mcp23x17_set_mode(IOExpanderInputs[i].io_expander_dev, IOExpanderInputs[i].io_port, MCP23X17_GPIO_INPUT);
        mcp23x17_set_pullup(IOExpanderInputs[i].io_expander_dev, IOExpanderInputs[i].io_port, true);
        mcp23x17_set_interrupt(IOExpanderInputs[i].io_expander_dev, IOExpanderInputs[i].io_port, MCP23X17_INT_ANY_EDGE);
    }

    // set initial io expander output states (low)
    for (int i = 0; i < sizeof(IOExpanderOutputs)/sizeof(IOExpanderOutputs[0]); i++) {
        mcp23x17_set_mode(IOExpanderOutputs[i].io_expander_dev, IOExpanderOutputs[i].io_port, MCP23X17_GPIO_OUTPUT);
        mcp23x17_set_level(IOExpanderOutputs[i].io_expander_dev, IOExpanderOutputs[i].io_port, false);  // switch (close contacts) relay
        vTaskDelay(pdMS_TO_TICKS(50));
        mcp23x17_set_level(IOExpanderOutputs[i].io_expander_dev, IOExpanderOutputs[i].io_port, true);  // switch (open contacts) relay
    }

    // run isr
    xTaskCreate(button_handler, "button_handler", 4096, NULL, 5, NULL);
    gpio_set_direction(IO_EXPANDER_1_INTA_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(IO_EXPANDER_1_INTB_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(IO_EXPANDER_2_INTA_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(IO_EXPANDER_2_INTB_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(IO_EXPANDER_3_INTA_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(IO_EXPANDER_3_INTB_PIN, GPIO_MODE_INPUT);

    gpio_set_intr_type(IO_EXPANDER_1_INTA_PIN, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(IO_EXPANDER_1_INTB_PIN, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(IO_EXPANDER_2_INTA_PIN, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(IO_EXPANDER_2_INTB_PIN, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(IO_EXPANDER_3_INTA_PIN, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(IO_EXPANDER_3_INTB_PIN, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(0);
    
    gpio_isr_handler_add(IO_EXPANDER_1_INTA_PIN, intr_handler, NULL);
    gpio_isr_handler_add(IO_EXPANDER_1_INTB_PIN, intr_handler, NULL);
    gpio_isr_handler_add(IO_EXPANDER_2_INTA_PIN, intr_handler, NULL);
    gpio_isr_handler_add(IO_EXPANDER_2_INTB_PIN, intr_handler, NULL);
    gpio_isr_handler_add(IO_EXPANDER_3_INTA_PIN, intr_handler, NULL);
    gpio_isr_handler_add(IO_EXPANDER_3_INTB_PIN, intr_handler, NULL);

    // initially clear the intcap registers by reading from them + set the initial button states / save them into the interrupt "buffer"
    mcp23x17_get_int_pin_states(&io_expander_dev1, &last_io_expander_states[0]);
    mcp23x17_get_int_pin_states(&io_expander_dev2, &last_io_expander_states[1]);
    mcp23x17_get_int_pin_states(&io_expander_dev3, &last_io_expander_states[2]);
}


void setup_gpios() {
    init_gpio_globals();
    init_io_expanders();
}