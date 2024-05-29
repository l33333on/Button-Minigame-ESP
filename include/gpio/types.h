#pragma once
#include "mcp23x17.h"

typedef struct {
    uint8_t io_port;
    mcp23x17_t *io_expander_dev;
    char *name;
    uint64_t last_trigger_time;
    uint8_t id;
} IOExpanderPort;

typedef enum
{
    RED = 0,
    GREEN = 1,
    BLUE = 2,
    DISABLED = 3
} Color;