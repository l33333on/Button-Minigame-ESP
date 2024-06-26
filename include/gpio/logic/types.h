#pragma once
#include "stdbool.h"
#include <stdint.h>


typedef struct {
    int id;
    bool status;
} LEDStatus;

typedef struct {  // considered using a hashmap, but decided not to for the sake of simplicity
    uint8_t sound_one[2];
    uint8_t sound_two[2];
    uint8_t sound_three[2];
    uint8_t sound_four[2];
    uint8_t sound_five[2];
    uint8_t sound_six[2];
    uint8_t sound_seven[2];
    uint8_t sound_eight[2];
} SoundButtonMapping;

typedef enum
{
    NONE_SELECTED = 0,
    FIRST_SELECTED = 1
}
CurrentSoundGameState;

typedef struct {
    uint8_t unused_btns[16];
    bool used_btns[16];  // already "cleared"
    uint8_t cleared_count;  // number of already "cleared" sounds
    CurrentSoundGameState state;
    SoundButtonMapping button_sound_mapping;
    uint8_t selected_button;
} GamemodeSoundRecognition;

typedef enum
{
    SLOW = 2000,
    MEDIUM = 1000,
    FAST = 500,
} Speed; 

typedef struct {
    
} GamemodeReaction;

typedef union {
    GamemodeSoundRecognition gamemode_sound_recognition_data;
    GamemodeReaction gamemode_reaction_data;
} GamemodeData;

typedef struct {
    uint8_t current_gamemode;
    bool initialized;
    bool is_playing_sound;
    Speed reaction_speed;  // it would be better to move it to the GamemodeReaction struct but Ill keep it here to be able to change the speed even though the game mode isn't selected
    GamemodeData current_gamemodedata;
    bool is_evaluating;
} Game;