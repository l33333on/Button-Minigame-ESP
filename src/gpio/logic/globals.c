#include "gpio/logic/globals.h"

Game game;

void init_gpio_logic_globals() {
    game.is_on = false;
    game.gamemode = 1;
}