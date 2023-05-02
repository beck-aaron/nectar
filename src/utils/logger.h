/*******************************************************************************
 * File:    logger.h
 * Author:  Aaron Beck
 * Date:    02/11/2023
 *
 * In order to access this logger output:
 *
 ******************************************************************************/
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <clock.h>
#include <vector.h>
#include <ctype.h>
#include <stdio.h>
#include <stdio_serial.h>

#define BASE_TWO 2

enum LOG_CODES
{
    DEBUG_LEVEL,
    WARNING_LEVEL,
    ERROR_LEVEL,
    TX_LEVEL,
    RX_LEVEL,
    ENCODER_LEVEL,
    DECODER_LEVEL,
};

#define DEBUG_COLOR         "\x1b[38;5;48m"     // green
#define WARNING_COLOR       "\x1b[38;5;11m"     // pale yellow
#define ERROR_COLOR         "\x1b[38;5;9m"      // red
#define HEX_COLOR           "\x1b[38;5;140m"    // purple
#define TX_COLOR            "\x1b[38;5;215m"    // orange
#define RX_COLOR            "\x1b[38;5;221m"    // yellow
#define DECODER_COLOR       "\x1b[38;5;45m"     // blue
#define ENCODER_COLOR       "\x1b[38;5;14m"     // light green
#define COLOR_RESET         "\x1b[0m"           // reset

#define LOGGER_LINE_LENGTH 0x10
#define LOGGER_START_MESSAGE "\r\n\
************** NECTAR LOGGER **************\r\n\
*       version:      0.0.1               *\r\n\
*     interface:      USART1              *\r\n\
*      baudrate:      115200              *\r\n\
*      compiled:      "__TIME__ "            *\r\n\
*******************************************\r\n"


// turns logging on or off to improve speed
#define LOG_EVERYTHING 0

#if LOG_EVERYTHING
#define LOG(LEVEL, ...)     \
    log_time();             \
    log_level(LEVEL);       \
    printf(__VA_ARGS__);    \
    printf(COLOR_RESET);    \
    log_endl();             \

#define LOGHEX(LEVEL, LABEL, BUFFER, LENGTH)    \
    log_time();                                 \
    log_level(LEVEL);                           \
    printf(LABEL);                              \
    log_hexdump((uint8_t*)BUFFER, LENGTH);      \
    printf(COLOR_RESET);                        \
    log_endl();                                 \

#define LOGBITS(LEVEL, LABEL, BUFFER, LENGTH)   \
    log_time();                                 \
    log_level(LEVEL);                           \
    printf(LABEL);                              \
    log_bitstring((uint8_t*)BUFFER, LENGTH);    \
    printf(COLOR_RESET);                        \
    log_endl();                                 \



/**
 * @brief Initializes a uart device for stdio. This enables
 * functions like printf to work nicely with a terminal output.
 * 
 * undefined behavior if used for mulitple peripherals!
 *
 * @param DEVICE - defined in conf_uart_serial.h
 */
#define serial_stdio_init(DEVICE)                                       \
    static const usart_serial_options_t DEVICE##_serial_options = {     \
        DEVICE##_UART_BAUDRATE,                                         \
        DEVICE##_UART_CHAR_LENGTH,                                      \
        DEVICE##_UART_PARITY,                                           \
        DEVICE##_UART_STOP_BITS,                                        \
    };                                                                  \
    stdio_serial_init(DEVICE##_UART, &DEVICE##_serial_options);

inline static void logger_init(void);
static void log_time(void);
inline static void log_level(uint8_t loglevel);
inline static void log_hexdump(uint8_t* hex, size_t size);
inline static void log_bitstring(uint8_t* bytes, size_t size);
inline static void log_endl(void);

inline static void logger_init(void)
{
    serial_stdio_init(LOGGER);
    puts(LOGGER_START_MESSAGE);
    LOG(DEBUG_LEVEL, "Initialized serial interface for logger.");

    time_t timestamp = time(NULL);
    LOG(DEBUG_LEVEL, "Timestamp: %s", asctime(gmtime(&timestamp)));
}

static void log_time(void)
{
    uint32_t hours, minutes, seconds;
    rtc_get_time(RTC, &hours, &minutes, &seconds);
    printf("%02lu:%02lu:%02lu", hours, minutes, seconds);
}

inline static void log_level(uint8_t loglevel)
{
    switch(loglevel)
    {
        case DEBUG_LEVEL: default:
            printf(" %s ", DEBUG_COLOR "[DEBUG]");
            break;
        case WARNING_LEVEL:
            printf(" %s ", WARNING_COLOR "[WARNING]");
            break;
        case ERROR_LEVEL:
            printf(" %s ", ERROR_COLOR "[ERROR]");
            break;
        case TX_LEVEL:
            printf(" %s ", TX_COLOR "[TX DATA]");
            break;
        case RX_LEVEL:
            printf(" %s ", RX_COLOR "[RX DATA]");
            break;
        case DECODER_LEVEL:
            printf(" %s ", DECODER_COLOR "[DECODER]");
            break;
        case ENCODER_LEVEL:
            printf(" %s ", ENCODER_COLOR "[ENCODER]");
            break;
    }
}

inline static void log_hexdump(uint8_t* byte, size_t size)
{
    vector_t line;
    vector_init(LOGGER_LINE_LENGTH, &line);
    log_endl();

    for (size_t j = 0; j < size; j += 0x10)
    {
        printf(" %04X: ", j);

        for (size_t i = 0; i < LOGGER_LINE_LENGTH; ++i)
        {
            if (i % 8 == 0 && i != 0)
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
}

inline static void log_bitstring(uint8_t* bytes, size_t size)
{
    uint32_t bitcount = size << 3;
    uint8_t bitstring[bitcount + 1];
    for (size_t i = 0; i < size; ++i)
    {
        for (uint8_t j = 0; j < 8; ++j)
        {
            bitstring[(i*8)+j] = (uint8_t)((bytes[i] >> j) & 0x1);
        }
    }

    for (size_t i = bitcount; i > 0; --i)
    {
        printf("%u", bitstring[i-1]);
    }
}

inline static void log_endl(void)
{
    printf("\r\n");
}

#else

#define LOG(LEVEL, ...)
#define LOGHEX(LEVEL, LABEL, BUFFER, LENGTH)
#define LOGBITS(LEVEL, LABEL, BUFFER, LENGTH)

#endif

#endif // _LOGGER_H_
