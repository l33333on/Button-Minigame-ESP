#include "mcp23x17.h"

extern mcp23x17_t io_expander_dev1;
extern mcp23x17_t io_expander_dev2;
extern mcp23x17_t io_expander_dev3;

extern mcp23x17_t *io_expander_devs[3];

void init_gpio_globals();