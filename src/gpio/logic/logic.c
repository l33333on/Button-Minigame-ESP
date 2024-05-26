#include "gpio/logic/logic.h"
#include "gpio/logic/globals.h"
#include "gpio/globals.h"

void on_off_btn_callback() {
    if (game.is_on) {  // shutdown
        // TODO -> play end sound 
        
        // TODO -> reset gamemode structs

        // reset leds
        for (int i = 0; i < sizeof(IOExpanderOutputs)/sizeof(IOExpanderOutputs[0]); i++) {
            mcp23x17_set_level(IOExpanderOutputs[i].io_expander_dev, IOExpanderOutputs[i].io_port, true);  // switch (open contacts) relay
        }
    }else {  // start

    }
    game.is_on = !game.is_on;
}