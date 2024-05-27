#pragma once
#include "mcp23x17.h"
#include "gpio/types.h"
#include "driver/uart.h"
#include "esp_err.h"

extern mcp23x17_t io_expander_dev1;
extern mcp23x17_t io_expander_dev2;
extern mcp23x17_t io_expander_dev3;

extern mcp23x17_t *io_expander_devs[3];

extern IOExpanderPort IOExpanderInputs[19];
extern IOExpanderPort IOExpanderOutputs[19];

extern uart_port_t uart_raspberry_port;
extern QueueHandle_t uart_queue;

esp_err_t init_gpio_globals();