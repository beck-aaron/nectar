/*******************************************************************************
 * File:    logger.c
 * Author:  Aaron Beck
 * Date:    02/11/2023
 *
 *
 * PLEASE be careful before using the "stdio_serial_init" function for any
 * other UART/USART interfaces.  The logger should be the primary use for
 * this function to avoid unexpected behavior.
 *
 ******************************************************************************/
#include "logger.h"

void logger_init(void)
{
    const usart_serial_options_t uart_serial_options =
    {
        .baudrate   = LOGGER_UART_BAUDRATE,
        .charlength = LOGGER_UART_CHAR_LENGTH,
        .paritytype = LOGGER_UART_PARITY,
        .stopbits   = LOGGER_UART_STOP_BITS
    };

    stdio_serial_init(LOGGER_UART, &uart_serial_options);
    puts(START_MESSAGE);
    LOG(DEBUG_LEVEL, "initialized logger");
}

void endl(void)
{
    printf("\r\n");
}

void log_level(uint8_t loglevel)
{
    switch(loglevel)
    {
        case DEBUG_LEVEL: default:
            printf( DEBUG_COLOR " [DEBUG] ");
            break;
        case WARNING_LEVEL:
            printf( WARNING_COLOR " [WARNING] ");
            break;
        case ERROR_LEVEL:
            printf( ERROR_COLOR " [ERROR] ");
            break;
        case TX_LEVEL:
            printf( TX_COLOR " [TX DATA] ");
            break;
        case RX_LEVEL:
            printf( RX_COLOR " [RX DATA] ");
            break;
    }
}

void log_header(uint8_t loglevel)
{
    log_time();
    log_level(loglevel);
}

void log_time(void)
{
    printf("%02x:%02x:%02x", 0x00, 0x00, 0x00);
}

void log_hexdump(const void* buffer, size_t size)
{
    const uint8_t* data = buffer;
    uint16_t byte_count = 0;
    for(size_t i = 0; i < size; ++i)
    {
        if (i % 16 == 0)
        {   // start newline & display count
            endl();
            printf(" %04X ", byte_count);
            byte_count += 0x10;
        }

        if (i % 8 == 0)
        {   // put space between columns
            putchar(' ');
        }

        printf("%02X ", data[i]);
    }

    endl();
}
