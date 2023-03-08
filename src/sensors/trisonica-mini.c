/*******************************************************************************
 * File:    trisonica-mini.c
 * Author:  Aaron Beck
 *
 * Drives the trisonica-mini sensor, attached to a UART interface defined by
 * config/conf_uart_serial.h
 *
 ******************************************************************************/
#include "trisonica-mini.h"

void tsm_init(void)
{
    usart_serial_options_t usart_options =
    {
        .baudrate   = TRISONICA_UART_BAUDRATE,
        .charlength = TRISONICA_UART_CHAR_LENGTH,
        .paritytype = TRISONICA_UART_PARITY,
        .stopbits   = TRISONICA_UART_STOP_BITS
    };

    usart_serial_init(TRISONICA_UART, &usart_options);
    LOG(DEBUG_LEVEL, "initialized trisonica-mini peripheral");
}

void tsm_read(void)
{
    uint8_t buffer[92];

    usart_serial_read_packet(TRISONICA_UART, buffer, sizeof(buffer));

    buffer[91] = '\0';
    printf("%s", buffer);
}
