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

// TODO
void log_time(void)
{
    printf("%02x:%02x:%02x", 0x00, 0x00, 0x00);
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

void log_hexdump(void* hex, size_t size)
{
    uint8_t* byte = hex;
    vector_t* line = vector_init(LINE_LENGTH);
    log_endl();

    for (size_t j = 0; j < size; j += 0x10)
    {
        printf(" %04X ", j);

        for (size_t i = 0; i < LINE_LENGTH; ++i)
        {
            if (i % 8 == 0)
                putchar(' ');

            if (i+j < size) 
                printf("%02X ", byte[i+j]);
            else
                printf("%3s", "");

            vector_push(&(byte)[i+j], sizeof(*byte), line);
        }

        putchar(' ');

        for (size_t i = 0; i < LINE_LENGTH; ++i)
        {
            if (iscntrl(line->data[i]))
            {
                putchar('.');
                continue;
            }

            putchar(line->data[i]);
        }

        vector_clear(line);
        log_endl();
    }

    vector_destroy(line);
    log_endl();
}

void log_endl(void)
{
    printf("\r\n");
}
