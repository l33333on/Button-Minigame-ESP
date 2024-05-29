#include "gpio/logic/globals.h"

Game game;
const char *cmd_play_correct;
const char *cmd_play_wrong;
const char *cmd_play_start;
const char *cmd_play_end;
const char *cmd_play_fast;
const char *cmd_play_normal;
const char *cmd_play_slow;
const char *cmd_play_sound_prefix;
const char *cmd_play_game_sound_prefix;
uint8_t blinking_leds[2];

void init_gpio_logic_globals() {
    game.current_gamemode = 1;
    game.initialized = false;
    game.is_playing_sound = false;
    game.reaction_speed = MEDIUM;
    game.is_evaluating = false;

    cmd_play_correct = "play_correct";
    cmd_play_wrong = "play_wrong";
    cmd_play_start = "play_start";
    cmd_play_end = "play_end";
    cmd_play_fast = "play_fast";
    cmd_play_normal = "play_normal";
    cmd_play_slow = "play_slow";
    cmd_play_sound_prefix = "play_sound_";
    cmd_play_game_sound_prefix = "play_game_";

    blinking_leds[0] = 0;
    blinking_leds[1] = 0;
}