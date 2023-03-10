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
    LOG(DEBUG_LEVEL, "Initialized logger.");
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
            printf(" %s  ", DEBUG_COLOR "[DEBUG]");
            break;
        case WARNING_LEVEL:
            printf(" %s  ", WARNING_COLOR "[WARNING]");
            break;
        case ERROR_LEVEL:
            printf(" %s  ", ERROR_COLOR "[ERROR]");
            break;
        case TX_LEVEL:
            printf(" %s  ", TX_COLOR "[TX DATA]");
            break;
        case RX_LEVEL:
            printf(" %s  ", RX_COLOR "[RX DATA]");
            break;
    }
}

void log_header(uint8_t loglevel)
{
    log_time();
    log_level(loglevel);
}

// TODO
void log_time(void)
{
    printf("%02x:%02x:%02x", 0x00, 0x00, 0x00);
}

void log_ascii(const uint8_t* str)
{
    putchar(' ');

    for (size_t i = 0; i < LINE_LENGTH; ++i)
    {
        if (iscntrl(str[i]))
        {
            putchar('.');
            continue;
        }

        putchar(str[i]);
    }
}

void log_hexdump(const void* buffer, size_t size)
{
    vector_t buffer_line = vector(LINE_LENGTH);

    endl();
    for (size_t j = 0; j < size; j += 0x10)
    {
        printf(" %04X ", j);
        for (size_t i = 0; i < LINE_LENGTH; ++i)
        {
            if (i % 8 == 0)
            {
                putchar(' ');
            }
            printf("%02X ", ((uint8_t*)buffer)[i+j]);
            buffer_line.push(&buffer_line, &((uint8_t*)buffer)[i+j], sizeof(uint8_t));
        }
        log_ascii(buffer_line.data);
        buffer_line.clear(&buffer_line);
        endl();
    }
    endl();
    buffer_line.destroy(&buffer_line);
}
