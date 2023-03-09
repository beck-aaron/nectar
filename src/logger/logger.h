/*******************************************************************************
 * File:    logger.h
 * Author:  Aaron Beck
 * Date:    02/11/2023
 *
 * Currently initializing logger to use USART2 ports, which correspond to
 * the TX2 & RX2 on the custom board we are using.
 *
 * In order to access this logging data:
 * OUTDATED: UPDATE THIS WITH CLEARER INSTRUCTIONS
 *
 * Evaluation board instructions:
 *
 * PTLS board instructions:
 *
 *  1. connect a USB - TTL/UART cable to the USART2 pins on the board.
 *
 *      we used a TTL-R232G-VREG3V3-WE cable by FTDI chip
 *
 *  2. connect to the usb port via terminal
 *      this can be done several ways, but the simplest
 *      is to use the "screen" or "picocom" command-line utilities.
 *
 *  $ screen /dev/tty.DEVICEID <parameters>
 *
 *      <baud_rate>           := amount of electrical signals received per second.
 *      <bits_per_byte>       := cs8 or cs7, transmission of 8 or 7 bits per byte
 *      <flow_control_tx>     := ixon or -ixon, enables/disables flow-control tx
 *      <flow_control_rx>     := ixoff or -ixoff, enables/disables flow-control rx
 *      <clear_keep_last_bit> := istrip or -istrip clear/keep eighth bit in each byte
 *
 *  example:
 *
 *  $ screen /dev/tty.usbserial-FT2VVX6F 115200 cs8 -ixon -ixoff -istrip
 *
 *  see screen manual for more details
 *
 ******************************************************************************/
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <asf.h>

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

#define START_MESSAGE "\r\n\
************** NECTAR LOGGER **************\r\n\
*       version:      0.0.1               *\r\n\
*     interface:      USART1              *\r\n\
*      baudrate:      115200              *\r\n\
*******************************************\r\n"

void logger_init(void);
void log_level(uint8_t loglevel);
void log_time(void);
void log_header(uint8_t loglevel);
void log_hexdump(const void* buffer, size_t size);
void endl(void);

#define LOG(LEVEL, ...) \
    log_header(LEVEL); \
    printf(__VA_ARGS__); \
    printf(COLOR_RESET); \
    endl();

#define LOGHEX(LEVEL, LABEL, BUFFER, LENGTH) \
    log_header(LEVEL); \
    printf(LABEL); \
    printf(COLOR_RESET HEX_COLOR); \
    log_hexdump(BUFFER, LENGTH); \
    printf(COLOR_RESET); \
    endl();

#endif // _LOGGER_H_
