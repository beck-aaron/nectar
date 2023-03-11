#ifndef _XBEE_H_
#define _XBEE_H_

#include <asf.h>
#include <logger.h>
#include "api_frames.h"
#include "at_commands.h"

#define XBEE_MAX_TX         256  // arbitrary values for now.
#define XBEE_MAX_RX         256  // represent tx & rx buffer size for each device
#define receive_xbee XBEE_UART_HANDLER

void test_xbee(void);
void encode_xbee(void);
void decode_xbee(void);
void configure_xbee(void);
void transmit_xbee(void);

typedef struct
{
    // XBee/XBee-PRO® S2C Zigbee® RF Module User Guide - pg 171
    uint8_t     delimiter;
    uint16_t    length;
    uint8_t     checksum;

    struct
    {
        uint8_t cmdID;

        union
        {
            at_command_t        at_command;
            transmit_request_t  transmit_request;
        };

    } api_frame;

    vector_t* tx_buffer;
    vector_t* rx_buffer;

    void (*test)(void);
    void (*encode)(void);
    void (*decode)(void);    // decodes the data within the rx buffer
    void (*configure)(void); // sets xbee device parameters
    void (*transmit)(void);  // transmits the encoded data within the payload

} xbee_t;

extern xbee_t xbee;

#endif /* _XBEE_H_ */
