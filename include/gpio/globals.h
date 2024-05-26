#pragma once
#include "mcp23x17.h"
#include "gpio/types.h"

extern mcp23x17_t io_expander_dev1;
extern mcp23x17_t io_expander_dev2;
extern mcp23x17_t io_expander_dev3;

extern mcp23x17_t *io_expander_devs[3];

extern QueueHandle_t xQueueGPIOEvt;

extern IOExpanderPort IOExpanderInputs[19];
extern IOExpanderPort IOExpanderOutputs[19];

void init_gpio_globals();