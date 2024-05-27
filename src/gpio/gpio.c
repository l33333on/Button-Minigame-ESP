#include <esp_log.h>
#include <mcp23x17.h>
#include <driver/gpio.h>
#include "configuration.h"
#include "gpio/globals.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include "gpio/logic/globals.h"
#include "gpio/logic/logic.h"
#include "string.h"
#include "gpio/serial.h"
#include "gpio/types.h"

#define BIT_BUTTON_CHANGED BIT(0)

static const char *TAG = "Button-Minigame-gpio";
static EventGroupHandle_t eg = NULL;
static uint16_t last_io_expander_states[3];


static void IRAM_ATTR intr_handler(void *arg)
{
    BaseType_t hp_task;

    if (xEventGroupSetBitsFromISR(eg, BIT_BUTTON_CHANGED, &hp_task) != pdFAIL)
        portYIELD_FROM_ISR(hp_task);
}

static int get_changed_bit_position(uint16_t current, uint16_t previous) {
    uint16_t from_1_to_0 = previous & ~current;  // bits that changed from 1 to 0 -> only the pins which got pressed instead of released (the interrupt gets raised on any edge)
    
    for (int i = 0; i < 16; i++) {
        if (from_1_to_0 & (1 << i)) {
            return i;
        }
    }
    return -1;
}

static void gpio_isr_task(void *pvParameters)
{
    uint16_t io_expander_states;

    for (;;) {
        if (xEventGroupWaitBits(eg, BIT_BUTTON_CHANGED, pdTRUE, pdTRUE, portMAX_DELAY) != BIT_BUTTON_CHANGED)
            continue;

        for (uint8_t i = 0; i < 3; i++) {
            mcp23x17_get_int_pin_states(io_expander_devs[i], &io_expander_states);
            // check if states differ from the previous states
            int changed_bit_position = get_changed_bit_position(io_expander_states, last_io_expander_states[i]);
            if (changed_bit_position == -1) {   // no change
                last_io_expander_states[i] = io_expander_states;  // also save the states when a button is released
                continue;
            }

            // get corresponding button
            for (int j = 0; j < sizeof(IOExpanderInputs)/sizeof(IOExpanderInputs[0]); j++) {
                if (IOExpanderInputs[j].io_expander_dev == io_expander_devs[i] && IOExpanderInputs[j].io_port == changed_bit_position) {
                    // "debounce" button
                    uint64_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;  // time in milliseconds
                    uint64_t time_since_last_trigger = (now - IOExpanderInputs[j].last_trigger_time);
                    if (time_since_last_trigger > DEBOUNCE_TIME_MS) {
                        IOExpanderInputs[j].last_trigger_time = now;
 
                        // call callback function

                        // these buttons should always work
                        if (strcmp(IOExpanderInputs[j].name, "ON_OFF_BTN") == 0) {
                            on_off_btn_callback();
                        }else
                        if (strcmp(IOExpanderInputs[j].name, "MODE_SEL_BTN") == 0) {
                            mode_sel_btn_callback();
                        }else
                        if (strcmp(IOExpanderInputs[j].name, "SPEED_SEL_BTN") == 0) {
                            speed_sel_btn_callback();
                        } else {
                            normal_btn_callback(IOExpanderInputs[j].name);
                        }
                    }
                    break;
                }
            }

            last_io_expander_states[i] = io_expander_states;
            break;
        }
    }
}

static void set_on_off_led(Color color) {
    mcp23x17_set_level(ON_OFF_LED_R.io_expander_dev, ON_OFF_LED_R.io_port, true);  // switch (open contacs) relay
    mcp23x17_set_level(ON_OFF_LED_G.io_expander_dev, ON_OFF_LED_G.io_port, true);  // switch (open contact) relay
    mcp23x17_set_level(ON_OFF_LED_B.io_expander_dev, ON_OFF_LED_B.io_port, true);  // switch (open contact) relay

    switch (color) {
        case RED:
            mcp23x17_set_level(ON_OFF_LED_R.io_expander_dev, ON_OFF_LED_R.io_port, false);  // switch (close contact) relay
            break;
        case GREEN:
            mcp23x17_set_level(ON_OFF_LED_G.io_expander_dev, ON_OFF_LED_G.io_port, false);  // switch (close contact) relay
            break;
        case BLUE:
            mcp23x17_set_level(ON_OFF_LED_B.io_expander_dev, ON_OFF_LED_B.io_port, false);  // switch (close contact) relay
            break;
        default:
            break;
    }
}

static void on_off_button_led_task(void *pvParameters) {
    uint16_t delay = 1000;
    bool is_blinking = false;
    Color color = DISABLED;

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(delay));
    
        if (game.is_on && game.initialized && !game.is_running) {
            // set BLUE blink delay to 1s
            delay = 1000;
            is_blinking = true;
        } else if(game.is_on && !game.initialized) {
            // set BLUE blink delay to 2s
            delay = 500;
            is_blinking = true;
        } else if (!game.is_on) {
            // disable on off led
            delay = 500;
            is_blinking = false;
        }

        if (!game. is_custom_led_state_showing) {  // for example when we indicate which game is running
            set_on_off_led(color);
            if (is_blinking) {
                color = color == DISABLED ? BLUE : DISABLED;
            } else {
                color = DISABLED;
            }
        }
    }
}

static void init_io_expanders() {
    ESP_ERROR_CHECK(i2cdev_init());
    ESP_ERROR_CHECK(mcp23x17_init_desc(&io_expander_dev1, IO_EXPANDER1_ADDR, 0, IO_EXPANDER_SDA_PIN, IO_EXPANDER_SCL_PIN));  //left
    io_expander_dev1.cfg.master.clk_speed = 40 * 1000;
    ESP_ERROR_CHECK(mcp23x17_init_desc(&io_expander_dev2, IO_EXPANDER2_ADDR, 0, IO_EXPANDER_SDA_PIN, IO_EXPANDER_SCL_PIN));  // middle
    io_expander_dev2.cfg.master.clk_speed = 40 * 1000;
    ESP_ERROR_CHECK(mcp23x17_init_desc(&io_expander_dev3, IO_EXPANDER3_ADDR, 0, IO_EXPANDER_SDA_PIN, IO_EXPANDER_SCL_PIN));  // right
    io_expander_dev3.cfg.master.clk_speed = 40 * 1000;
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
    xTaskCreate(gpio_isr_task, "gpio_isr_task", 4096, NULL, 3, NULL);
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

    xTaskCreate(on_off_button_led_task, "on_off_button_led_task", 4096, NULL, 4, NULL);
}


void setup_gpios() {
    ESP_ERROR_CHECK(init_gpio_globals());
    ESP_ERROR_CHECK(init_raspberry_serial_port());
    init_io_expanders();
    init_gpio_logic_globals();
}