/*******************************************************************************
 * File:    coz-ira.c
 * Author:  Aaron Beck
 *
 * Drives the coz-ira sensor, attached to a UART interface defined by
 * config/conf_uart_serial.h
 *
 * We configure the sensor in the initialization function to stream data.
 * This prevents us sending a request for data and not receiving a response.
 *
 * However, this means that the cpu is bounded by waiting for this IO operation.
 * TODO: figure out how to use DMA in order to remove this wait time for the CPU
 *
 ******************************************************************************/
#include "coz-ir.h"

static uint8_t response[34];

void coz_ir_init(void)
{
    usart_serial_options_t usart_options =
    {
        .baudrate   = COZ_IR_UART_BAUDRATE,
        .charlength = COZ_IR_UART_CHAR_LENGTH,
        .paritytype = COZ_IR_UART_PARITY,
        .stopbits   = COZ_IR_UART_STOP_BITS
    };

    // put a null terminating character at the end of our string.
    response[33] = '\0';
    usart_serial_init(COZ_IR_UART, &usart_options);
    LOG(DEBUG_LEVEL, "initialized telaire-t6615 interface");
}

void coz_ir_read(void)
{
    usart_serial_read_packet(COZ_IR_UART, response, sizeof(response)-1);
    response[0] = ' ';
    LOG(DEBUG_LEVEL, "COZ-IR response: %s", response);
}

