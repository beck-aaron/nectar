/*******************************************************************************
 * File:    xbee.h
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#ifndef _XBEE_H_
#define _XBEE_H_

#include <asf.h>
#include <state.h>
#include <dma.h>
#include <logger.h>
#include "api_frames.h"
#include "at_commands.h"

#define XBEE_MAX_TX         256  // arbitrary values for now.
#define XBEE_MAX_RX         256  // represent tx & rx buffer size for each device
#define xbee_uart_handler   XBEE_UART_HANDLER

typedef struct
{
    // XBee/XBee-PRO® S2C Zigbee® RF Module User Guide - pg 171
    uint8_t     delimiter;
    uint16_t    length;
    api_frame_t api_frame;
    uint8_t     checksum;

    volatile device_state_t state;

    vector_t tx_buffer;
    vector_t rx_buffer;
    volatile serial_state_t tx_state;
    volatile serial_state_t rx_state;

    void (*test)(void);
    void (*transmit)(void);  // transmits the encoded data within the payload
    void (*receive)(void);   // checks if UART is not ready and logs received data
    void (*encode)(void);
    void (*decode)(void);    // decodes the data within the rx buffer
    void (*configure)(void); // sets xbee device parameters

} xbee_t;
extern xbee_t xbee;

// public functions here
void xbee_init(void);
void xbee_test(void);
void xbee_transmit(void);
void xbee_receive(void);
void xbee_encode(void);
void xbee_decode(void);
void xbee_configure(void);

//void xbee_force_receive(void);


#endif /* _XBEE_H_ */
