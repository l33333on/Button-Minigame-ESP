#pragma once
#include "stdint.h"
#include "gpio/types.h"

void on_off_btn_callback();
void normal_btn_callback(const IOExpanderPort *button);
void mode_sel_btn_callback();
void speed_sel_btn_callback();
void init_game();