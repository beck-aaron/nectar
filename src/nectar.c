/*******************************************************************************
 * File:    nectar.c
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#include "nectar.h"

inline void nectar_init(nectar_t* nectar)
{
    logger_init();
    xdmac_init();
    devices_init(
        &nectar->xbee,
        &nectar->coz_ir,
        &nectar->telaire,
        &nectar->trisonica
    );
    LED_On(LED0);

    LOG(DEBUG_LEVEL, "[Nectar] Successfully initialized all drivers and connected devices.");
    LOG(DEBUG_LEVEL, "[Nectar] Starting data collection and transmission procedure.");

    delay_s(5);
}

// NOTE: this is the buffer that gets sent in the transmit request payload
uint8_t buffer[] = "hello world";

// we should pop from our queue of payloads only when xbee.tx_state == SERIAL_IDLE
inline void nectar_transmit(nectar_t* nectar)
{
    if (nectar->xbee.state == DEVICE_DISCONNECTED)
    {
        LOG(ERROR_LEVEL, "[Nectar] xbee communication device disconnected, please connect and reset the program!");
        return;
    }

    // TODO: if queue is empty, return;
    if (nectar->xbee.tx_state == SERIAL_IDLE)
    {
        // pop from subpayload queue
        // set transmit request with appropriate data
        xbee_set_transmit_request(buffer, sizeof(buffer)-1, &nectar->xbee.api_frame);
    }

    xbee_transmit(&nectar->xbee);
}

inline void nectar_receive(nectar_t* nectar)
{
    // todo: collect data from sensors
    xbee_receive(&nectar->xbee);
    //coz_ir.receive();
    //telaire.receive();
}

void nectar_compile(nectar_t* nectar)
{
    // format data from sensors into subpayloads, then payloads
}
