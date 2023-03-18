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

#include <stdio.h>
#include <ctype.h>
#include <vector.h>
#include <rtc.h>

#define DEBUG_LEVEL         0u
#define WARNING_LEVEL       1u
#define ERROR_LEVEL         2u
#define TX_LEVEL            3u
#define RX_LEVEL            4u

#define DEBUG_COLOR         "\x1b[38;5;48m"     // green debug color
#define WARNING_COLOR       "\x1b[38;5;11m"     // yellow warning color
#define ERROR_COLOR         "\x1b[38;5;9m"      // red error color
#define HEX_COLOR           "\x1b[38;5;140m"    // purple hexdump color
#define TX_COLOR            "\x1b[38;5;215m"    // orange tx color
#define RX_COLOR            "\x1b[38;5;221m"    // yellow rx color
#define COLOR_RESET         "\x1b[0m"           // reset default color

#define LOGGER_LINE_LENGTH 0x10
#define LOGGER_START_MESSAGE "\r\n\
************** NECTAR LOGGER **************\r\n\
*       version:      0.0.1               *\r\n\
*     interface:      USART1              *\r\n\
*      baudrate:      115200              *\r\n\
*      compiled:      "__TIME__ "            *\r\n\
*******************************************\r\n"

void log_time(void);
void log_level(uint8_t loglevel);
void log_hexdump(uint8_t* hex, size_t size);
void log_endl(void);

#define LOG(LEVEL, ...) \
    log_time(); \
    log_level(LEVEL); \
    printf(__VA_ARGS__); \
    printf(COLOR_RESET); \
    log_endl();

#define LOGHEX(LEVEL, LABEL, BUFFER, LENGTH) \
    log_time(); \
    log_level(LEVEL); \
    printf(LABEL); \
    printf(COLOR_RESET HEX_COLOR); \
    log_hexdump((uint8_t*)BUFFER, LENGTH); \
    printf(COLOR_RESET); \
    log_endl();

#endif // _LOGGER_H_
