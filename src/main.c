#include "gpio/gpio.h"
#include "audio/pcm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio/logic/globals.h"
#include "gpio/logic/logic.h"

static const char *TAG = "Button-Minigame-main";

void app_main()
{
    setup_gpios();
    init_game(1);
    vTaskDelay(pdMS_TO_TICKS(60 * 1000)); // initially wait 60s to make sure the raspberry started properly
    game.initialized = true;  // TODO -> initially illuminate the top left corner to show that the first game is running + play game end or start sound
}