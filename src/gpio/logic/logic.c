#include "gpio/logic/logic.h"
#include "gpio/logic/globals.h"
#include "gpio/globals.h"
#include "gpio/serial.h"
#include "string.h"
#include "esp_random.h"
#include "gpio/logic/sound.h"

void on_off_btn_callback() {
    if (game.is_on) {  // shutdown
        // play end sound
        play_sound(cmd_play_end);

        // reset leds
        for (int i = 0; i < sizeof(IOExpanderOutputs)/sizeof(IOExpanderOutputs[0]); i++) {
            mcp23x17_set_level(IOExpanderOutputs[i].io_expander_dev, IOExpanderOutputs[i].io_port, true);  // switch (open contacts) relay
        }
    }else {  // start
        // TODO -> reset game structs

        // play start sound
        play_sound(cmd_play_start);
    }
    game.is_on = !game.is_on;
}

void mode_sel_btn_callback() {
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
void speed_sel_btn_callback() {

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

void init_game(uint8_t gamemode) {  // function which initialized the GamemodeData structure of the game structure
    switch (gamemode) {
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

            printf("%d, %d\n", sound_button_mapping.sound_one[0], sound_button_mapping.sound_one[1]);
            printf("%d, %d\n", sound_button_mapping.sound_two[0], sound_button_mapping.sound_two[1]);
            printf("%d, %d\n", sound_button_mapping.sound_three[0], sound_button_mapping.sound_three[1]);
            printf("%d, %d\n", sound_button_mapping.sound_four[0], sound_button_mapping.sound_four[1]);
            printf("%d, %d\n", sound_button_mapping.sound_five[0], sound_button_mapping.sound_five[1]);
            printf("%d, %d\n", sound_button_mapping.sound_six[0], sound_button_mapping.sound_six[1]);
            printf("%d, %d\n", sound_button_mapping.sound_seven[0], sound_button_mapping.sound_seven[1]);
            printf("%d, %d\n", sound_button_mapping.sound_eight[0], sound_button_mapping.sound_eight[1]);

            GamemodeSoundRecognition gamemode_soundrecognition = {
                .unused_btns = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
                .used_btns = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
                .button_sound_mapping = sound_button_mapping
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

void normal_btn_callback(const char *button_name) {
    // these buttons should only work when the game is running
    if (game.is_on && game.is_running && !game.is_custom_led_state_showing) {
        switch (game.current_gamemode) {
            case 1:

                break;
            case 2:
                break;  // not implemented yet
            case 3:
                break;
            case 4:  // not implemented yet
                break;
            default:
                break;
        }

        char suffix[4];

        // can only be one of the 1-16 button inputs
        switch (strlen(button_name)) {
            case 10:
                suffix[0] = button_name[5];
                suffix[1] = '\0';
                break;
            case 11:
                suffix[0] = button_name[5];
                suffix[1] = button_name[6];
                suffix[2] = '\0';
                break;
            default:
                return;
        }
        char buf_cmd_play_sound[20];
        strcpy(buf_cmd_play_sound, cmd_play_sound_prefix);
        strcat(buf_cmd_play_sound, suffix);
        printf("%s\n", buf_cmd_play_sound);

        play_sound(buf_cmd_play_sound);
    }
}