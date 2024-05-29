#include "gpio/logic/logic.h"
#include "gpio/logic/globals.h"
#include "gpio/globals.h"
#include "gpio/serial.h"
#include "string.h"
#include "esp_random.h"
#include "gpio/logic/sound.h"
#include "configuration.h"
#include "gpio/gpio.h"

static void reset_leds() {
    for (int i = 0; i < sizeof(IOExpanderOutputs)/sizeof(IOExpanderOutputs[0]); i++) {
        mcp23x17_set_level(IOExpanderOutputs[i].io_expander_dev, IOExpanderOutputs[i].io_port, true);  // switch (open contacts) relay
    }
    blinking_leds[0] = 0;
    blinking_leds[1] = 0;

}

void mode_sel_btn_callback() {
    game.current_gamemode = game.current_gamemode == 1 ? 3 : 1;  // only game 1 and 3 are implemented
    // game.current_gamemode = game.current_gamemode + 1 % 4;
    // create new game struct
    init_game();
    reset_leds();

    // play game sound
    char buf_cmd_play_sound[20];
    char number[4];
    sprintf(number, "%d", game.current_gamemode);
    strcpy(buf_cmd_play_sound, cmd_play_game_sound_prefix);
    strcat(buf_cmd_play_sound, number);
    play_sound(buf_cmd_play_sound);
}

void speed_sel_btn_callback() {
    switch (game.reaction_speed) {
        case SLOW:
            game.reaction_speed = MEDIUM;
            play_sound(cmd_play_normal);
            break;
        case MEDIUM:
            game.reaction_speed = FAST;
            play_sound(cmd_play_fast);
            break;
        case FAST:
            game.reaction_speed = SLOW;
            play_sound(cmd_play_slow);
            break;
        default:
            break;
    }
}

static void shuffle_array(uint8_t *array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + esp_random() / (UINT32_MAX / (n - i) + 1);
            uint8_t temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}

void init_game() {  // function which initialized the GamemodeData structure of the game structure
    switch (game.current_gamemode) {
        case 1:
            SoundButtonMapping sound_button_mapping;

            // randomly assign buttons to sounds
            uint8_t numbers[16];
            for (uint8_t i = 0; i < 16; ++i) {
                numbers[i] = i + 1;
            }
            shuffle_array(numbers, 16);
            for (int i = 0; i < 2; ++i) {
                sound_button_mapping.sound_one[i] = numbers[i];
                sound_button_mapping.sound_two[i] = numbers[i + 2];
                sound_button_mapping.sound_three[i] = numbers[i + 4];
                sound_button_mapping.sound_four[i] = numbers[i + 6];
                sound_button_mapping.sound_five[i] = numbers[i + 8];
                sound_button_mapping.sound_six[i] = numbers[i + 10];
                sound_button_mapping.sound_seven[i] = numbers[i + 12];
                sound_button_mapping.sound_eight[i] = numbers[i + 14];
            }

            GamemodeSoundRecognition gamemode_soundrecognition = {
                .used_btns = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
                .unused_btns = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
                .cleared_count = 0,
                .state = NONE_SELECTED,
                .button_sound_mapping = sound_button_mapping,
                .selected_button = 0
                };
            GamemodeData gamemodeData;
            gamemodeData.gamemode_sound_recognition_data = gamemode_soundrecognition;
            game.current_gamemodedata = gamemodeData;
            break;
        case 2:  // not implemented yet
            break;  
        case 3:

            break;
        case 4:  // not implemented yet
            break;
        default:
            break;
    }
}

static void play_sound_for_button(const IOExpanderPort *button) {
    char buf_cmd_play_sound[20];
    strcpy(buf_cmd_play_sound, cmd_play_sound_prefix);

    for (int i = 0; i < 2; i++) {
        if (game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_one[i] == button->id) {
            strcat(buf_cmd_play_sound, "1");
            break;
        }else if (game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_two[i] == button->id) {
            strcat(buf_cmd_play_sound, "2");
            break;
        } else if (game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_three[i] == button->id) {
            strcat(buf_cmd_play_sound, "3");
            break;
        } else if (game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_four[i] == button->id) {
            strcat(buf_cmd_play_sound, "4");
            break;
        } else if (game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_five[i] == button->id) {
            strcat(buf_cmd_play_sound, "5");
            break;
        } else if (game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_six[i] == button->id) {
            strcat(buf_cmd_play_sound, "6");
            break;
        } else if (game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_seven[i] == button->id) {
            strcat(buf_cmd_play_sound, "7");
            break;
        } else if (game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_eight[i] == button->id) {
            strcat(buf_cmd_play_sound, "8");
            break;
        }
    }
    play_sound(buf_cmd_play_sound);
}

static bool is_all_correct() {
    for (int i = 1; i < 17; i++) {
        if (!game.current_gamemodedata.gamemode_sound_recognition_data.used_btns[i]) {
            return false;
        }
    }
    return true;
}

static bool is_correct(uint8_t id_1, uint8_t id_2) {
    uint8_t *sounds[] = {
        game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_one,
        game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_two,
        game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_three,
        game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_four,
        game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_five,
        game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_six,
        game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_seven,
        game.current_gamemodedata.gamemode_sound_recognition_data.button_sound_mapping.sound_eight,
    };
    for (int i = 0; i < 8; i++) {
        if ((sounds[i][0] == id_1 && sounds[i][1] == id_2) ||
            (sounds[i][0] == id_2 && sounds[i][1] == id_1)) {
            return true;
        }
    }
    return false;
}

static void set_led_state(uint8_t id, bool state) {
    for (int i = 0; i < sizeof(IOExpanderOutputs)/sizeof(IOExpanderOutputs[0]); i++) {
        if (IOExpanderOutputs[i].id == id) {
            mcp23x17_set_level(IOExpanderOutputs[i].io_expander_dev, IOExpanderOutputs[i].io_port, !state);
            return;
        }
    }
}

static void game_1_btn_callback(const IOExpanderPort *button) {
    switch (game.current_gamemodedata.gamemode_sound_recognition_data.state) {
        case NONE_SELECTED:
            // blink button
            blinking_leds[0] = button->id;
            blinking_leds[1] = 0;
            play_sound_for_button(button);  // making sure to start blinking before playing the sound
            game.current_gamemodedata.gamemode_sound_recognition_data.selected_button = button->id;
            game.current_gamemodedata.gamemode_sound_recognition_data.state = FIRST_SELECTED;
            break;
        case FIRST_SELECTED:
            game.is_evaluating = true;

            // blink button
            blinking_leds[1] = button->id;
            play_sound_for_button(button);  // making sure to start blinking before playing the sound

            vTaskDelay(pdMS_TO_TICKS(1500));
            
            // check if correct
            if (is_correct(blinking_leds[0], blinking_leds[1])) {
                // play sound, show green led
                play_sound(cmd_play_correct);
                set_on_off_led(GREEN);
                // illuminate both leds
                set_led_state(blinking_leds[0], true);
                set_led_state(blinking_leds[1], true);
                game.current_gamemodedata.gamemode_sound_recognition_data.used_btns[blinking_leds[0]] = true;
                game.current_gamemodedata.gamemode_sound_recognition_data.used_btns[blinking_leds[1]] = true;
                printf("%d\n", blinking_leds[0]);
                printf("%d\n", blinking_leds[1]);
                blinking_leds[0] = 0;
                blinking_leds[1] = 0;

                if (is_all_correct()) {
                    // restart game
                    vTaskDelay(pdMS_TO_TICKS(5 * 1000));
                    play_sound(cmd_play_end);
                    // blink buttons 3 times
                    bool state = false;
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < sizeof(IOExpanderOutputs)/sizeof(IOExpanderOutputs[0]); j++) {
                            mcp23x17_set_level(IOExpanderOutputs[j].io_expander_dev, IOExpanderOutputs[j].io_port, state);
                            vTaskDelay(pdMS_TO_TICKS(333));
                        }
                    }
                    init_game();
                    reset_leds();
                    set_on_off_led(BLUE);
                    break;
                }
            } else {  // is wrong
                // play sound, show red led
                play_sound(cmd_play_wrong);
                set_on_off_led(RED);
                set_led_state(blinking_leds[0], false);
                set_led_state(blinking_leds[1], false);
                blinking_leds[0] = 0;
                blinking_leds[1] = 0;
            }
            vTaskDelay(pdMS_TO_TICKS(3 * 1000));
            set_on_off_led(BLUE);
            blinking_leds[0] = 0;
            blinking_leds[1] = 0;
            game.current_gamemodedata.gamemode_sound_recognition_data.selected_button = 0;
            game.current_gamemodedata.gamemode_sound_recognition_data.state = NONE_SELECTED;
            break;
    }
    game.is_evaluating = false;
}

static void game_3_btn_callback(const IOExpanderPort *button) {
    
}

static bool is_button_is_already_used(const IOExpanderPort *button) {
    return game.current_gamemodedata.gamemode_sound_recognition_data.used_btns[button->id];
}

void normal_btn_callback(const IOExpanderPort *button) {
    if (is_button_is_already_used(button)) {
        printf("already used\n");
        return;
    }

    switch (game.current_gamemode) {
        case 1:
            game_1_btn_callback(button);
            break;
        case 2:  // not implemented yet
            break;  
        case 3:
            game_3_btn_callback(button);
            break;
        case 4:  // not implemented yet
            break;
        default:
            break;
    }

    // // can only be one of the 1-16 button inputs
    // switch (strlen(button_name)) {
    //     case 10:
    //         suffix[0] = button_name[5];
    //         suffix[1] = '\0';
    //         break;
    //     case 11:
    //         suffix[0] = button_name[5];
    //         suffix[1] = button_name[6];
    //         suffix[2] = '\0';
    //         break;
    //     default:
    //         return;
    // }
    // char buf_cmd_play_sound[20];
    // strcpy(buf_cmd_play_sound, cmd_play_sound_prefix);
    // strcat(buf_cmd_play_sound, suffix);

    // play_sound(buf_cmd_play_sound);
}