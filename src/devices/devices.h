/*******************************************************************************
 * File:    devices.h
 * Author:  Aaron Beck
 *
 * Stores the data structures for each device used with the Nectar driver
 * and a function to initialize all the peripherals used for external devices.
 *
 ******************************************************************************/

#ifndef DEVICES_H_
#define DEVICES_H_

#include <clock.h>
#include "coz_ir/coz_ir.h"
#include "telaire/telaire.h"
#include "trisonica/trisonica.h"
#include "xbee/xbee.h"

/**
 * @brief Depends on definitions conf_uart_serial.h
 * To add a new device, simply call one of these macros
 * or create a new one for a different peripheral.
 *
 * One should note that behavior is undefined when multiple
 * calls to init_serial_stdio are invoked since this presumedly
 * overwrites the stdio fd from the previous peripheral.
 */
void devices_init(void);

/**
 * @brief Initializes a uart device for stdio. This enables
 * functions like printf to work nicely with a terminal output.
 *
 * @param DEVICE - defined in conf_uart_serial.h
 */
#define serial_stdio_init(DEVICE) \
    const usart_serial_options_t DEVICE##_serial_options = { \
        DEVICE##_UART_BAUDRATE, \
        DEVICE##_UART_CHAR_LENGTH, \
        DEVICE##_UART_PARITY, \
        DEVICE##_UART_STOP_BITS, \
    }; \
    stdio_serial_init(DEVICE##_UART, &DEVICE##_serial_options);

/**
 * @brief Initializes a uart device for serial IO.
 * This sets up the appropriate uart/usart peripheral for data transmission
 * and retrieval.
 *
 * @param DEVICE - defined in conf_uart_serial.h
 */
#define serial_uart_init(DEVICE) \
    usart_serial_options_t DEVICE##_serial_options = { \
        DEVICE##_UART_BAUDRATE, \
        DEVICE##_UART_CHAR_LENGTH, \
        DEVICE##_UART_PARITY, \
        DEVICE##_UART_STOP_BITS, \
    }; \
    usart_serial_init(DEVICE##_UART, &DEVICE##_serial_options);

#endif /* DEVICES_H_ */
