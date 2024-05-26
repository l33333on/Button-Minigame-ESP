#pragma once
#include "stdbool.h"
#include <stdint.h>

typedef struct {
    bool is_on;
    uint8_t gamemode;
} Game;

typedef struct {
    int id;
    bool status;
} LEDStatus;

typedef struct {
    
} GamemodeSoundRecognition;