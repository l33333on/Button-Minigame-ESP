#include "gpio/gpio.h"
#include "filesystem/sd_card.h"
#include "audio/pcm.h"

static const char *TAG = "Button-Minigame-main";

void app_main()
{
    setup_gpios();
    setup_sd_card();
    setup_pcm_audio();
}