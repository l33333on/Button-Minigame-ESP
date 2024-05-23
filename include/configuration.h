#include "gpio/types.h"
#include "gpio/globals.h"

//----------------------------------------------------------------
// general
#define DEBOUNCE_TIME_MS 10

#define IO_EXPANDER1_ADDR 0x20
#define IO_EXPANDER2_ADDR 0x21
#define IO_EXPANDER3_ADDR 0x23

#define IO_EXPANDER_SDA_PIN GPIO_NUM_21
#define IO_EXPANDER_SCL_PIN GPIO_NUM_22

#define SD_CARD_SCLK_PIN GPIO_NUM_25
#define SD_CARD_MOSI_PIN GPIO_NUM_26
#define SD_CARD_MISO_PIN GPIO_NUM_23
#define SD_CARD_CS_PIN GPIO_NUM_13

#define LINE_BCK_PIN GPIO_NUM_33
#define LINE_DIN_PIN GPIO_NUM_32
#define LINE_LCK_PIN GPIO_NUM_27

#define IO_EXPANDER_1_INTA_PIN GPIO_NUM_16
#define IO_EXPANDER_1_INTB_PIN GPIO_NUM_17

#define IO_EXPANDER_2_INTA_PIN GPIO_NUM_18
#define IO_EXPANDER_2_INTB_PIN GPIO_NUM_19

#define IO_EXPANDER_3_INTA_PIN GPIO_NUM_34
#define IO_EXPANDER_3_INTB_PIN GPIO_NUM_35

//----------------------------------------------------------------
// IO expander #1 inputs
const IOExpanderPort SPEED_SEL_BTN = {12, &io_expander_dev1, "SPEED_SEL_BTN"};
const IOExpanderPort MODE_SEL_BTN = {13, &io_expander_dev1, "MODE_SEL_BTN"};
const IOExpanderPort INPUT13_BTN = {14, &io_expander_dev1, "INPUT13_BTN"};
const IOExpanderPort INPUT14_BTN = {15, &io_expander_dev1, "INPUT14_BTN"};

//----------------------------------------------------------------
// IO expander #1 outputs
const IOExpanderPort INPUT16_LED = {0, &io_expander_dev1, "INPUT16_LED"};
const IOExpanderPort INPUT15_LED = {1, &io_expander_dev1, "INPUT15_LED"};
const IOExpanderPort INPUT13_LED = {3, &io_expander_dev1, "INPUT13_LED"};
const IOExpanderPort INPUT14_LED = {2, &io_expander_dev1, "INPUT14_LED"};

//----------------------------------------------------------------
// IO expander #2 inputs
const IOExpanderPort INPUT15_BTN = {8, &io_expander_dev2, "INPUT15_BTN"};
const IOExpanderPort INPUT16_BTN = {9, &io_expander_dev2, "INPUT16_BTN"};
const IOExpanderPort INPUT9_BTN = {10, &io_expander_dev2, "INPUT9_BTN"};
const IOExpanderPort INPUT10_BTN = {11, &io_expander_dev2, "INPUT10_BTN"};
const IOExpanderPort INPUT11_BTN = {12, &io_expander_dev2, "INPUT11_BTN"};
const IOExpanderPort INPUT12_BTN = {13, &io_expander_dev2, "INPUT12_BTN"};
const IOExpanderPort INPUT5_BTN = {14, &io_expander_dev2, "INPUT5_BTN"};
const IOExpanderPort INPUT6_BTN = {15, &io_expander_dev2, "INPUT6_BTN"};

//----------------------------------------------------------------
// IO expander #2 outputs
const IOExpanderPort INPUT8_LED = {0, &io_expander_dev2, "INPUT8_LED"};
const IOExpanderPort INPUT7_LED = {1, &io_expander_dev2, "INPUT7_LED"};
const IOExpanderPort INPUT6_LED = {2, &io_expander_dev2, "INPUT6_LED"};
const IOExpanderPort INPUT5_LED = {3, &io_expander_dev2, "INPUT5_LED"};
const IOExpanderPort INPUT12_LED = {4, &io_expander_dev2, "INPUT12_LED"};
const IOExpanderPort INPUT11_LED = {5, &io_expander_dev2, "INPUT11_LED"};
const IOExpanderPort INPUT10_LED = {6, &io_expander_dev2, "INPUT10_LED"};
const IOExpanderPort INPUT9_LED = {7, &io_expander_dev2, "INPUT9_LED"};

//----------------------------------------------------------------
// IO expander #3 inputs
const IOExpanderPort INPUT7_BTN = {8, &io_expander_dev3, "INPUT7_BTN"};
const IOExpanderPort INPUT8_BTN = {9, &io_expander_dev3, "INPUT8_BTN"};
const IOExpanderPort INPUT1_BTN = {10, &io_expander_dev3, "INPUT1_BTN"};
const IOExpanderPort INPUT2_BTN = {11, &io_expander_dev3, "INPUT2_BTN"};
const IOExpanderPort INPUT3_BTN = {12, &io_expander_dev3, "INPUT3_BTN"};
const IOExpanderPort INPUT4_BTN = {13, &io_expander_dev3, "INPUT4_BTN"};
const IOExpanderPort ON_OFF_BTN = {14, &io_expander_dev3, "ON_OFF_BTN"};

//----------------------------------------------------------------
// IO expander #3 outputs
const IOExpanderPort ON_OFF_LED_B = {1, &io_expander_dev3, "ON_OFF_LED_B"};
const IOExpanderPort ON_OFF_LED_G = {2, &io_expander_dev3, "ON_OFF_LED_G"};
const IOExpanderPort ON_OFF_LED_R = {3, &io_expander_dev3, "ON_OFF_LED_R"};
const IOExpanderPort INPUT4_LED = {4, &io_expander_dev3, "INPUT4_LED"};
const IOExpanderPort INPUT3_LED = {5, &io_expander_dev3, "INPUT3_LED"};
const IOExpanderPort INPUT2_LED = {6, &io_expander_dev3, "INPUT2_LED"};
const IOExpanderPort INPUT1_LED = {7, &io_expander_dev3, "INPUT1_LED"};

//----------------------------------------------------------------
const IOExpanderPort IOExpanderInputs[20] = {
    ON_OFF_BTN,
    MODE_SEL_BTN,
    SPEED_SEL_BTN,
    INPUT1_BTN,
    INPUT2_BTN,
    INPUT3_BTN,
    INPUT4_BTN,
    INPUT5_BTN,
    INPUT6_BTN,
    INPUT7_BTN,
    INPUT8_BTN,
    INPUT9_BTN,
    INPUT10_BTN,
    INPUT11_BTN,
    INPUT12_BTN,
    INPUT13_BTN,
    INPUT14_BTN,
    INPUT15_BTN,
    INPUT16_BTN,
};
const IOExpanderPort IOExpanderOutputs[19] = {
    ON_OFF_LED_R,
    ON_OFF_LED_G,
    ON_OFF_LED_B,
    INPUT1_LED,
    INPUT2_LED,
    INPUT3_LED,
    INPUT4_LED,
    INPUT5_LED,
    INPUT6_LED,
    INPUT7_LED,
    INPUT8_LED,
    INPUT9_LED,
    INPUT10_LED,
    INPUT11_LED,
    INPUT12_LED,
    INPUT13_LED,
    INPUT14_LED,
    INPUT15_LED,
    INPUT16_LED
};