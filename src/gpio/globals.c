#include "gpio/globals.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "gpio/types.h"
#include "configuration.h"

mcp23x17_t io_expander_dev1;  // left
mcp23x17_t io_expander_dev2;  // middle
mcp23x17_t io_expander_dev3;  // right

mcp23x17_t *io_expander_devs[3];

IOExpanderPort IOExpanderInputs[18];
IOExpanderPort IOExpanderOutputs[19];

uart_port_t uart_raspberry_port;
QueueHandle_t uart_queue;

esp_err_t init_gpio_globals() {
    memset(&io_expander_dev1, 0, sizeof(mcp23x17_t));
    memset(&io_expander_dev2, 0, sizeof(mcp23x17_t));
    memset(&io_expander_dev3, 0, sizeof(mcp23x17_t));
    io_expander_dev1.cfg.scl_pullup_en = false;
    io_expander_dev1.cfg.sda_pullup_en = false;
    io_expander_dev2.cfg.scl_pullup_en = false;
    io_expander_dev2.cfg.sda_pullup_en = false;
    io_expander_dev3.cfg.scl_pullup_en = false;
    io_expander_dev3.cfg.sda_pullup_en = false;
    io_expander_devs[0] = &io_expander_dev1;
    io_expander_devs[1] = &io_expander_dev2;
    io_expander_devs[2] = &io_expander_dev3;
    
    IOExpanderInputs[0] = MODE_SEL_BTN;
    IOExpanderInputs[1] = SPEED_SEL_BTN;
    IOExpanderInputs[2] = INPUT1_BTN;
    IOExpanderInputs[3] = INPUT2_BTN;
    IOExpanderInputs[4] = INPUT3_BTN;
    IOExpanderInputs[5] = INPUT4_BTN;
    IOExpanderInputs[6] = INPUT5_BTN;
    IOExpanderInputs[7] = INPUT6_BTN;
    IOExpanderInputs[8] = INPUT7_BTN;
    IOExpanderInputs[9] = INPUT8_BTN;
    IOExpanderInputs[10] = INPUT9_BTN;
    IOExpanderInputs[11] = INPUT10_BTN;
    IOExpanderInputs[12] = INPUT11_BTN;
    IOExpanderInputs[13] = INPUT12_BTN;
    IOExpanderInputs[14] = INPUT13_BTN;
    IOExpanderInputs[15] = INPUT14_BTN;
    IOExpanderInputs[16] = INPUT15_BTN;
    IOExpanderInputs[17] = INPUT16_BTN;

    IOExpanderOutputs[0] = ON_OFF_LED_R;
    IOExpanderOutputs[1] = ON_OFF_LED_G;
    IOExpanderOutputs[2] = ON_OFF_LED_B;
    IOExpanderOutputs[3] = INPUT1_LED;
    IOExpanderOutputs[4] = INPUT2_LED;
    IOExpanderOutputs[5] = INPUT3_LED;
    IOExpanderOutputs[6] = INPUT4_LED;
    IOExpanderOutputs[7] = INPUT5_LED;
    IOExpanderOutputs[8] = INPUT6_LED;
    IOExpanderOutputs[9] = INPUT7_LED;
    IOExpanderOutputs[10] = INPUT8_LED;
    IOExpanderOutputs[11] = INPUT9_LED;
    IOExpanderOutputs[12] = INPUT10_LED;
    IOExpanderOutputs[13] = INPUT11_LED;
    IOExpanderOutputs[14] = INPUT12_LED;
    IOExpanderOutputs[15] = INPUT13_LED;
    IOExpanderOutputs[16] = INPUT14_LED;
    IOExpanderOutputs[17] = INPUT15_LED;
    IOExpanderOutputs[18] = INPUT16_LED;
    
    return ESP_OK;
}