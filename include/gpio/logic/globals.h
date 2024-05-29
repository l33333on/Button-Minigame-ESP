#pragma once
#include "gpio/logic/types.h"

extern Game game;
extern const char *cmd_play_correct;
extern const char *cmd_play_wrong;
extern const char *cmd_play_start;
extern const char *cmd_play_end;
extern const char *cmd_play_fast;
extern const char *cmd_play_normal;
extern const char *cmd_play_slow;
extern const char *cmd_play_sound_prefix;
extern const char *cmd_play_game_sound_prefix;
extern uint8_t blinking_leds[2];

void init_gpio_logic_globals();