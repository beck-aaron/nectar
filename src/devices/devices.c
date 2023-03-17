/*******************************************************************************
 * File:    devices.c
 * Author:  Aaron Beck
 *
 * Stores the data structures for each device used with the Nectar driver
 * and a function to initialize all the peripherals used for external devices.
 *
 ******************************************************************************/

#include "devices.h"
#include "dma.h"

inline static void xbee_init(void);
inline static void logger_init(void);
inline static void coz_ir_init(void);
inline static void telaire_init(void);
inline static void trisonica_init(void);

void devices_init(void)
{
    logger_init();
    xbee_init();
//  telaire_init();
//  trisonica_init();
//  coz_ir_init();
}

inline static void xbee_init(void)
{
    vector_init(XBEE_MAX_TX, &xbee.tx_buffer);
    vector_init(XBEE_MAX_RX, &xbee.rx_buffer);
    serial_uart_init(XBEE);
    
    xdmac_configure_peripheral_to_memory(
        XBEE_UART,
        XBEE_HWID_RX,
        xbee.rx_buffer.data,
        XBEE_MAX_RX,
        XBEE_CHANNEL_RX
    );

//  xdmac_enable_peripheral_to_memory(XBEE_CHANNEL_RX);

    LOG(DEBUG_LEVEL, "Initialized serial interface for xbee.");
}

inline static void logger_init(void)
{
    serial_stdio_init(LOGGER);
    puts(LOGGER_START_MESSAGE);
    LOG(DEBUG_LEVEL, "Initialized serial interface for logger.");

    time_t timestamp = time(NULL);
    LOG(DEBUG_LEVEL, "Timestamp: %s", asctime(localtime(&timestamp)));
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

