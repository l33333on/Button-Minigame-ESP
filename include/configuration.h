#pragma once
#include "gpio/globals.h"
#include "gpio/types.h"


//----------------------------------------------------------------
// general
#define DEBOUNCE_TIME_MS 200

#define IO_EXPANDER1_ADDR 0x20
#define IO_EXPANDER2_ADDR 0x21
#define IO_EXPANDER3_ADDR 0x23

#define IO_EXPANDER_SDA_PIN GPIO_NUM_21
#define IO_EXPANDER_SCL_PIN GPIO_NUM_22

#define IO_EXPANDER_1_INTA_PIN GPIO_NUM_16
#define IO_EXPANDER_1_INTB_PIN GPIO_NUM_17

#define IO_EXPANDER_2_INTA_PIN GPIO_NUM_18
#define IO_EXPANDER_2_INTB_PIN GPIO_NUM_19

#define IO_EXPANDER_3_INTA_PIN GPIO_NUM_34
#define IO_EXPANDER_3_INTB_PIN GPIO_NUM_35

#define UART_RASPBERRY_TX_PIN GPIO_NUM_13
#define UART_RASPBERRY_RX_PIN GPIO_NUM_23
#define UART_RASPBERRY_RTS_PIN GPIO_NUM_33
#define UART_RASPBERRY_CTS_PIN GPIO_NUM_32

//----------------------------------------------------------------
// IO expander #1 inputs
extern IOExpanderPort SPEED_SEL_BTN;
extern IOExpanderPort MODE_SEL_BTN;
extern IOExpanderPort INPUT13_BTN;
extern IOExpanderPort INPUT14_BTN;

//----------------------------------------------------------------
// IO expander #1 outputs
extern const IOExpanderPort INPUT16_LED;
extern const IOExpanderPort INPUT15_LED;
extern const IOExpanderPort INPUT13_LED;
extern const IOExpanderPort INPUT14_LED;

//----------------------------------------------------------------
// IO expander #2 inputs
extern IOExpanderPort INPUT15_BTN;
extern IOExpanderPort INPUT16_BTN;
extern IOExpanderPort INPUT9_BTN;
extern IOExpanderPort INPUT10_BTN;
extern IOExpanderPort INPUT11_BTN;
extern IOExpanderPort INPUT12_BTN;
extern IOExpanderPort INPUT5_BTN;
extern IOExpanderPort INPUT6_BTN;

//----------------------------------------------------------------
// IO expander #2 outputs
extern const IOExpanderPort INPUT8_LED;
extern const IOExpanderPort INPUT7_LED;
extern const IOExpanderPort INPUT6_LED;
extern const IOExpanderPort INPUT5_LED;
extern const IOExpanderPort INPUT12_LED;
extern const IOExpanderPort INPUT11_LED;
extern const IOExpanderPort INPUT10_LED;
extern const IOExpanderPort INPUT9_LED;

//----------------------------------------------------------------
// IO expander #3 inputs
extern IOExpanderPort INPUT7_BTN;
extern IOExpanderPort INPUT8_BTN;
extern IOExpanderPort INPUT1_BTN;
extern IOExpanderPort INPUT2_BTN;
extern IOExpanderPort INPUT3_BTN;
extern IOExpanderPort INPUT4_BTN;
extern IOExpanderPort ON_OFF_BTN;

//----------------------------------------------------------------
// IO expander #3 outputs
extern const IOExpanderPort ON_OFF_LED_B;
extern const IOExpanderPort ON_OFF_LED_G;
extern const IOExpanderPort ON_OFF_LED_R;
extern const IOExpanderPort INPUT4_LED;
extern const IOExpanderPort INPUT3_LED;
extern const IOExpanderPort INPUT2_LED;
extern const IOExpanderPort INPUT1_LED;