/*******************************************************************************
 * File:    logger.c
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#include "logger.h"

void log_time(void)
{
    uint32_t hours, minutes, seconds;
    rtc_get_time(RTC, &hours, &minutes, &seconds);
    printf("%02lu:%02lu:%02lu", hours, minutes, seconds);
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

void log_hexdump(uint8_t* byte, size_t size)
{
    vector_t line;
    vector_init(LOGGER_LINE_LENGTH, &line);
    log_endl();

    for (size_t j = 0; j < size; j += 0x10)
    {
        printf(" %04X: ", j);

        for (size_t i = 0; i < LOGGER_LINE_LENGTH; ++i)
        {
            if (i % 8 == 0)
                printf(" - ");

            if (i+j < size) 
                printf("%02X ", byte[i+j]);
            else
                printf("%3s", "");

            vector_push(&(byte)[i+j], sizeof(*byte), &line);
        }

        printf(" | ");

        for (size_t i = 0; i < LOGGER_LINE_LENGTH; ++i)
        {
            if (iscntrl(line.data[i]))
            {
                putchar('.');
                continue;
            }

            putchar(line.data[i]);
        }

        vector_clear(&line);
        log_endl();
    }

    vector_destroy(&line);
    log_endl();
}

void log_endl(void)
{
    printf("\r\n");
}
