#include "configuration.h"

//----------------------------------------------------------------
// IO expander #1 inputs
IOExpanderPort SPEED_SEL_BTN = {12, &io_expander_dev1, "SPEED_SEL_BTN", 0};
IOExpanderPort MODE_SEL_BTN = {13, &io_expander_dev1, "MODE_SEL_BTN", 0};
IOExpanderPort INPUT13_BTN = {14, &io_expander_dev1, "INPUT13_BTN", 0};
IOExpanderPort INPUT14_BTN = {15, &io_expander_dev1, "INPUT14_BTN", 0};

//----------------------------------------------------------------
// IO expander #1 outputs
const IOExpanderPort INPUT16_LED = {0, &io_expander_dev1, "INPUT16_LED", 0};
const IOExpanderPort INPUT15_LED = {1, &io_expander_dev1, "INPUT15_LED", 0};
const IOExpanderPort INPUT13_LED = {3, &io_expander_dev1, "INPUT13_LED", 0};
const IOExpanderPort INPUT14_LED = {2, &io_expander_dev1, "INPUT14_LED", 0};

//----------------------------------------------------------------
// IO expander #2 inputs
IOExpanderPort INPUT15_BTN = {8, &io_expander_dev2, "INPUT15_BTN", 0};
IOExpanderPort INPUT16_BTN = {9, &io_expander_dev2, "INPUT16_BTN", 0};
IOExpanderPort INPUT9_BTN = {10, &io_expander_dev2, "INPUT9_BTN", 0};
IOExpanderPort INPUT10_BTN = {11, &io_expander_dev2, "INPUT10_BTN", 0};
IOExpanderPort INPUT11_BTN = {12, &io_expander_dev2, "INPUT11_BTN", 0};
IOExpanderPort INPUT12_BTN = {13, &io_expander_dev2, "INPUT12_BTN", 0};
IOExpanderPort INPUT5_BTN = {14, &io_expander_dev2, "INPUT5_BTN", 0};
IOExpanderPort INPUT6_BTN = {15, &io_expander_dev2, "INPUT6_BTN", 0};

//----------------------------------------------------------------
// IO expander #2 outputs
const IOExpanderPort INPUT8_LED = {0, &io_expander_dev2, "INPUT8_LED", 0};
const IOExpanderPort INPUT7_LED = {1, &io_expander_dev2, "INPUT7_LED", 0};
const IOExpanderPort INPUT6_LED = {2, &io_expander_dev2, "INPUT6_LED", 0};
const IOExpanderPort INPUT5_LED = {3, &io_expander_dev2, "INPUT5_LED", 0};
const IOExpanderPort INPUT12_LED = {4, &io_expander_dev2, "INPUT12_LED", 0};
const IOExpanderPort INPUT11_LED = {5, &io_expander_dev2, "INPUT11_LED", 0};
const IOExpanderPort INPUT10_LED = {6, &io_expander_dev2, "INPUT10_LED", 0};
const IOExpanderPort INPUT9_LED = {7, &io_expander_dev2, "INPUT9_LED", 0};

//----------------------------------------------------------------
// IO expander #3 inputs
IOExpanderPort INPUT7_BTN = {8, &io_expander_dev3, "INPUT7_BTN", 0};
IOExpanderPort INPUT8_BTN = {9, &io_expander_dev3, "INPUT8_BTN", 0};
IOExpanderPort INPUT1_BTN = {10, &io_expander_dev3, "INPUT1_BTN", 0};
IOExpanderPort INPUT2_BTN = {11, &io_expander_dev3, "INPUT2_BTN", 0};
IOExpanderPort INPUT3_BTN = {12, &io_expander_dev3, "INPUT3_BTN", 0};
IOExpanderPort INPUT4_BTN = {13, &io_expander_dev3, "INPUT4_BTN", 0};
IOExpanderPort ON_OFF_BTN = {14, &io_expander_dev3, "ON_OFF_BTN", 0};

//----------------------------------------------------------------
// IO expander #3 outputs
const IOExpanderPort ON_OFF_LED_B = {1, &io_expander_dev3, "ON_OFF_LED_B", 0};
const IOExpanderPort ON_OFF_LED_G = {2, &io_expander_dev3, "ON_OFF_LED_G", 0};
const IOExpanderPort ON_OFF_LED_R = {3, &io_expander_dev3, "ON_OFF_LED_R", 0};
const IOExpanderPort INPUT4_LED = {4, &io_expander_dev3, "INPUT4_LED", 0};
const IOExpanderPort INPUT3_LED = {5, &io_expander_dev3, "INPUT3_LED", 0};
const IOExpanderPort INPUT2_LED = {6, &io_expander_dev3, "INPUT2_LED", 0};
const IOExpanderPort INPUT1_LED = {7, &io_expander_dev3, "INPUT1_LED", 0};