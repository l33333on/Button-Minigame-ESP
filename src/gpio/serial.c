#include "gpio/serial.h"
#include "configuration.h"
#include "gpio/serial.h"

esp_err_t init_raspberry_serial_port() {
    uart_raspberry_port = UART_NUM_1;
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    if (uart_param_config(uart_raspberry_port, &uart_config) != ESP_OK) return ESP_FAIL;
    if (uart_set_pin(uart_raspberry_port, UART_RASPBERRY_TX_PIN, UART_RASPBERRY_RX_PIN, UART_RASPBERRY_RTS_PIN, UART_RASPBERRY_CTS_PIN)) return ESP_FAIL;
    if (uart_driver_install(uart_raspberry_port, 1024, 1024, 10, &uart_queue, 0)) return ESP_FAIL;

    return ESP_OK;
}