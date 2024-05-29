#include "gpio/gpio.h"
#include "audio/pcm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio/logic/globals.h"
#include "gpio/logic/logic.h"
#include "gpio/logic/sound.h"
#include "string.h"

static const char *TAG = "Button-Minigame-main";

void app_main()
{
    setup_gpios();
    init_game();
    vTaskDelay(pdMS_TO_TICKS(10 * 1000)); // initially wait 60s to make sure the raspberry started properly
    init_game();
    game.initialized = true;
}