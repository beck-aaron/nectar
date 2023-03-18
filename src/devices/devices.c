/*******************************************************************************
 * File:    devices.c
 * Author:  Aaron Beck
 *
 * Stores the data structures for each device used with the Nectar driver
 * and a function to initialize all the peripherals used for external devices.
 *
 ******************************************************************************/
#include "devices.h"

inline static void xbee_init(void);
inline static void logger_init(void);
inline static void coz_ir_init(void);
inline static void telaire_init(void);
inline static void trisonica_init(void);

void devices_init(void)
{
    pmc_enable_periph_clk(ID_XDMAC);
    logger_init();
    xbee_init();
    telaire_init();
    trisonica_init();
    coz_ir_init();
}

inline static void xbee_init(void)
{
    vector_init(XBEE_MAX_TX, &xbee.tx_buffer);
    vector_init(XBEE_MAX_RX, &xbee.rx_buffer);
    serial_uart_init(XBEE);

    // configure xbee here

    LOG(DEBUG_LEVEL, "Initialized serial interface for xbee.");
}

inline static void logger_init(void)
{
    serial_stdio_init(LOGGER);
    puts(LOGGER_START_MESSAGE);
    LOG(DEBUG_LEVEL, "Initialized serial interface for logger.");

    time_t timestamp = time(NULL);
    LOG(DEBUG_LEVEL, "Timestamp: %s", asctime(gmtime(&timestamp)));
}

inline static void telaire_init(void)
{
    //todo: init buffers here
    serial_uart_init(TELAIRE);

    // configure telaire here

    LOG(DEBUG_LEVEL, "Initialized serial interface for telaire.");
}

inline static void trisonica_init(void)
{
    //todo: init buffers here
    serial_uart_init(TRISONICA);

    // configure trisonica here

    LOG(DEBUG_LEVEL, "Initialized serial interface for trisonica mini.");
}

inline static void coz_ir_init(void)
{
    //todo: init buffers here
    serial_uart_init(COZ_IR);

    // configure coz_ir here

    LOG(DEBUG_LEVEL, "Initialized serial interface for coz-ir.");
}

