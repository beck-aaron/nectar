/*******************************************************************************
 * File:    devices.c
 * Author:  Aaron Beck
 *
 * Stores the data structures for each device used with the Nectar driver
 * and a function to initialize all the peripherals used for external devices.
 *
 ******************************************************************************/

#include "devices.h"

inline static void logger_init(void);
inline static void xbee_init(void);
inline static void coz_ir_init(void);
inline static void telaire_init(void);
inline static void trisonica_init(void);

void devices_init(void)
{
    logger_init();
    xbee_init();
    telaire_init();
    trisonica_init();
    coz_ir_init();
}

inline static void xbee_init(void)
{
    // TODO setup XDMAC for device
    // use interrupt to LOG tranfered data
    // or set a status flag
    vector_init(XBEE_MAX_TX, &xbee.tx_buffer);
    vector_init(XBEE_MAX_RX, &xbee.rx_buffer);
    usart_enable_interrupt(XBEE_UART, US_IER_RXRDY);
    usart_enable_interrupt(XBEE_UART, US_IER_OVRE);

    irq_register_handler(XBEE_UART_IRQn, 1);
    serial_uart_init(XBEE);

    xbee.configure();
    LOG(DEBUG_LEVEL, "Initialized serial interface for xbee.");
}

inline static void logger_init(void)
{
    serial_stdio_init(LOGGER);
    puts(LOGGER_START_MESSAGE);
    LOG(DEBUG_LEVEL, "Initialized serial interface for logger.");
}

inline static void telaire_init(void)
{
    serial_uart_init(TELAIRE);
    LOG(DEBUG_LEVEL, "Initialized serial interface for telaire.");
}

inline static void trisonica_init(void)
{
    serial_uart_init(TRISONICA);
    LOG(DEBUG_LEVEL, "Initialized serial interface for trisonica mini.");
}

inline static void coz_ir_init(void)
{
    serial_uart_init(COZ_IR);
    LOG(DEBUG_LEVEL, "Initialized serial interface for coz-ir.");
}
