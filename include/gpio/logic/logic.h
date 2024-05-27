#pragma once
#include "stdint.h"

void on_off_btn_callback();
void normal_btn_callback(const char *button_name);
void mode_sel_btn_callback();
void speed_sel_btn_callback();
void init_game(uint8_t gamemode);