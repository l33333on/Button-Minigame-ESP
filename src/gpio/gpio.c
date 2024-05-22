#include <esp_log.h>
#include <mcp23x17.h>
#include <driver/gpio.h>
#include "configuration.h"
#include "gpio/globals.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

static const char *TAG = "Button-Minigame-gpio";

static EventGroupHandle_t eg = NULL;
#define BIT_BUTTON_CHANGED BIT(0)
static void IRAM_ATTR intr_handler(void *arg)
{
    // On interrupt set bit in event group
    BaseType_t hp_task;
    if (xEventGroupSetBitsFromISR(eg, BIT_BUTTON_CHANGED, &hp_task) != pdFAIL)
        portYIELD_FROM_ISR(hp_task);
}

void print_binary(uint8_t num) {
    // Bitmaske zum Überprüfen jedes Bits
    uint16_t mask = 0b10000000;

    // Iteriere über jedes Bit der Zahl
    for (int i = 0; i < 8; i++) {
        // Überprüfe, ob das aktuelle Bit gesetzt ist
        if (num & mask) {
            printf("1");
        } else {
            printf("0");
        }

        // Verschiebe die Maske um ein Bit nach rechts für das nächste Bit
        mask >>= 1;
    }
}

void button_handler(void *pvParameters)
{
    while (1)
    {
        // wait for BIT_BUTTON_CHANGED, clear it on exit
        if (xEventGroupWaitBits(eg, BIT_BUTTON_CHANGED, pdTRUE, pdTRUE, portMAX_DELAY) != BIT_BUTTON_CHANGED)
            continue;
        // OK, we got this bit set
        ESP_LOGI(TAG, "Button was pressed!");
        uint8_t test;
        uint8_t test1;
        mcp23x17_get_int_a_pin_states(&io_expander_dev1, &test);
        mcp23x17_get_int_b_pin_states(&io_expander_dev1, &test1);
        print_binary(test);
        print_binary(test1);
        printf("\n");
        mcp23x17_get_int_a_pin_states(&io_expander_dev2, &test);
        mcp23x17_get_int_b_pin_states(&io_expander_dev2, &test1);
        print_binary(test);
        print_binary(test1);
        printf("\n");
        mcp23x17_get_int_a_pin_states(&io_expander_dev3, &test);
        mcp23x17_get_int_b_pin_states(&io_expander_dev3, &test1);
        print_binary(test);
        print_binary(test1);
        printf("\n");
        printf("\n");
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

    // set initial io expander input states (pull up resistor, isr, etc.)
    eg = xEventGroupCreate();
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

    // initially clear the intcap registers by reading from them
    mcp23x17_get_int_a_pin_states(&io_expander_dev1, NULL);
    mcp23x17_get_int_a_pin_states(&io_expander_dev2, NULL);
    mcp23x17_get_int_a_pin_states(&io_expander_dev3, NULL);
    mcp23x17_get_int_b_pin_states(&io_expander_dev1, NULL);
    mcp23x17_get_int_b_pin_states(&io_expander_dev2, NULL);
    mcp23x17_get_int_b_pin_states(&io_expander_dev3, NULL);

    // TODO -> remove
    mcp23x17_set_mode(&io_expander_dev1, 5, MCP23X17_GPIO_INPUT);
    mcp23x17_set_pullup(&io_expander_dev1, 5, true);
    mcp23x17_set_interrupt(&io_expander_dev1, 5, MCP23X17_INT_ANY_EDGE);
    mcp23x17_set_mode(&io_expander_dev1, 6, MCP23X17_GPIO_INPUT);
    mcp23x17_set_pullup(&io_expander_dev1, 6, true);
    mcp23x17_set_interrupt(&io_expander_dev1, 6, MCP23X17_INT_ANY_EDGE);

}


void setup_gpios() {
    init_gpio_globals();
    init_io_expanders();
}