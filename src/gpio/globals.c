#include "gpio/globals.h"
#include <string.h>

mcp23x17_t io_expander_dev1;  // left
mcp23x17_t io_expander_dev2;  // middle
mcp23x17_t io_expander_dev3;  // right

void init_gpio_globals() {
    memset(&io_expander_dev1, 0, sizeof(mcp23x17_t));
    memset(&io_expander_dev2, 0, sizeof(mcp23x17_t));
    memset(&io_expander_dev3, 0, sizeof(mcp23x17_t));
    io_expander_dev1.cfg.scl_pullup_en = false;
    io_expander_dev1.cfg.sda_pullup_en = false;
    io_expander_dev2.cfg.scl_pullup_en = false;
    io_expander_dev2.cfg.sda_pullup_en = false;
    io_expander_dev3.cfg.scl_pullup_en = false;
    io_expander_dev3.cfg.sda_pullup_en = false;
}