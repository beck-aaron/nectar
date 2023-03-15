#ifndef _XBEE_H_
#define _XBEE_H_

#include <asf.h>
#include <logger.h>
#include "api_frames.h"
#include "at_commands.h"

#define XBEE_MAX_TX         256  // arbitrary values for now.
#define XBEE_MAX_RX         256  // represent tx & rx buffer size for each device
#define xbee_uart_handler   XBEE_UART_HANDLER

void xbee_test(void);
void xbee_encode(void);
void xbee_decode(void);
void xbee_transmit(void);
void xbee_receive(void);
void xbee_force_receive(void);
void xbee_configure(void);

typedef struct
{
    volatile bool received_api_frame;
    uint64_t coordinator_address_64;
    uint16_t coordinator_address_16;

    // XBee/XBee-PRO® S2C Zigbee® RF Module User Guide - pg 171
    uint8_t     delimiter;
    uint16_t    length;
    api_frame_t api_frame;
    uint8_t     checksum;

    vector_t tx_buffer;
    vector_t rx_buffer;

    void (*test)(void);
    void (*encode)(void);
    void (*decode)(void);    // decodes the data within the rx buffer
    void (*transmit)(void);  // transmits the encoded data within the payload
    void (*receive)(void);   // checks if UART is not ready and logs received data
    void (*configure)(void); // sets xbee device parameters

} xbee_t;

extern xbee_t xbee;

#endif /* _XBEE_H_ */
