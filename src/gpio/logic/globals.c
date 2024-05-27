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

void init_gpio_logic_globals() {
    game.is_on = false;
    game.current_gamemode = 1;
    game.initialized = false;
    game.is_running = false;
    game.is_custom_led_state_showing = false;
    game.is_playing_sound = false;
    game.reaction_speed = MEDIUM;

    cmd_play_correct = "play_correct";
    cmd_play_wrong = "play_wrong";
    cmd_play_start = "play_start";
    cmd_play_end = "play_end";
    cmd_play_fast = "play_fast";
    cmd_play_normal = "play_normal";
    cmd_play_slow = "play_slow";
    cmd_play_sound_prefix = "play_sound_";
}