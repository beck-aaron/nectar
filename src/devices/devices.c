/*******************************************************************************
 * File:    devices.c
 * Author:  Aaron Beck
 *
 * Stores the data structures for each device used with the Nectar driver
 * and a function to initialize all the peripherals used for external devices.
 *
 ******************************************************************************/

#include "devices.h"

void devices_init(void)
{
    init_serial_stdio(LOGGER);
    LOG(DEBUG_LEVEL, "Initialized Logger serial interface.");

    init_serial_uart(TELAIRE);
    LOG(DEBUG_LEVEL, "Initialized Telaire serial interface.");

    init_serial_uart(TRISONICA);
    LOG(DEBUG_LEVEL, "Initialized Trisonica Mini serial interface.");

    init_serial_uart(COZ_IR);
    LOG(DEBUG_LEVEL, "Initialized CoZ-IR serial interface.");

    init_xbee();
}

void init_xbee(void)
{
    xbee.tx_buffer = vector_init(XBEE_MAX_TX);
    xbee.rx_buffer = vector_init(XBEE_MAX_RX);
    usart_enable_interrupt(XBEE_UART, US_IER_RXRDY);
    irq_register_handler(XBEE_UART_IRQ, 1);
    init_serial_uart(XBEE);

    xbee.configure();
    LOG(DEBUG_LEVEL, "Initialized Xbee serial interface.");
}
