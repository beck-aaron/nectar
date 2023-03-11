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

#include <asf.h>
#include <logger.h>
#include "xbee/xbee.h"

#define TELAIRE_MAX_TX      256  // TODO: determine how much memory our
#define TELAIRE_MAX_RX      256  // program has access to...
#define TRISONICA_MAX_TX    256
#define TRISONICA_MAX_RX    256
#define COZ_IR_MAX_TX       256
#define COZ_IR_MAX_RX       256

#define MAX_PAYLOAD_SIZE    0x54

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
#define init_serial_stdio(DEVICE) \
    const usart_serial_options_t DEVICE##_serial_options = { \
        DEVICE##_UART_BAUDRATE, \
        DEVICE##_UART_CHAR_LENGTH, \
        DEVICE##_UART_PARITY, \
        DEVICE##_UART_STOP_BITS, \
    }; \
    stdio_serial_init(DEVICE##_UART, &DEVICE##_serial_options); \
    puts(DEVICE##_START_MESSAGE);

/**
 * @brief Initializes a uart device for serial IO.
 * This sets up the appropriate uart/usart peripheral for data transmission
 * and retrieval.
 *
 * @param DEVICE - defined in conf_uart_serial.h
 */
#define init_serial_uart(DEVICE) \
    usart_serial_options_t DEVICE##_serial_options = { \
        DEVICE##_UART_BAUDRATE, \
        DEVICE##_UART_CHAR_LENGTH, \
        DEVICE##_UART_PARITY, \
        DEVICE##_UART_STOP_BITS, \
    }; \
    usart_serial_init(DEVICE##_UART, &DEVICE##_serial_options);

typedef struct
{
    // specific data types go here
    uint8_t tx_data[TRISONICA_MAX_TX];
    uint8_t rx_data[TRISONICA_MAX_RX];

    void (*encode)(void);
    void (*decode)(void);
    void (*calibrate)(void);

} trisonica_mini_t;
extern trisonica_mini_t trisonica_mini;

typedef struct
{
    // specific data types go here
    uint8_t tx_data[TELAIRE_MAX_TX];
    uint8_t rx_data[TELAIRE_MAX_RX];

    void (*encode)(void);
    void (*decode)(void);
    void (*calibrate)(void);

} telaire_t;
extern telaire_t telaire;


typedef struct
{
    // specific data types go here
    uint8_t tx_data[COZ_IR_MAX_TX];
    uint8_t rx_data[COZ_IR_MAX_RX];

    // sensor functions here
    void (*encode)(void);
    void (*decode)(void);
    void (*calibrate)(void);

} coz_ir_t;
extern coz_ir_t coz_ir;

#endif /* DEVICES_H_ */
