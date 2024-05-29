#include "gpio/logic/tasks.h"
#include "gpio/logic/globals.h"
#include "configuration.h"

void blink_buttons_task(void *pvParameters) {
    bool state = false;

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(500));

        for (int i = 0; i < 2; i++) {
            if (blinking_leds[i] == 0)
                continue;
            for (int j = 0; j < sizeof(IOExpanderOutputs)/sizeof(IOExpanderOutputs[0]); j++) {
                if (IOExpanderOutputs[j].id == blinking_leds[i]) {
                    mcp23x17_set_level(IOExpanderOutputs[j].io_expander_dev, IOExpanderOutputs[j].io_port, state);
                }
            }
        }
        state = !state;
    }
}