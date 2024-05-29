#include "gpio/logic/sound.h"
#include "gpio/logic/globals.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio/globals.h"
#include "string.h"


void play_sound(const char *command) {
    game.is_playing_sound = true;
    uart_write_bytes(uart_raspberry_port, command, strlen(command));
    vTaskDelay(pdMS_TO_TICKS(2 * 1000)); // wait 2 seconds
    game.is_playing_sound = false;
}
