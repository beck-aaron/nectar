/*******************************************************************************
 * File:    xbee.h
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#ifndef _XBEE_H_
#define _XBEE_H_

#include <dma.h>
#include <state.h>
#include <timer.h>
#include <logger.h>
#include "api_frames.h"

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
    serial_state_t tx_state;
    serial_state_t rx_state;

} xbee_t;

// public functions here
void xbee_init(xbee_t* xbee);
void xbee_transmit(xbee_t* xbee);
void xbee_receive(xbee_t* xbee);
void xbee_encode(xbee_t* xbee);
void xbee_decode(xbee_t* xbee);

//void xbee_force_receive(void);


#endif /* _XBEE_H_ */
