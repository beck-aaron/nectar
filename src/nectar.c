/*******************************************************************************
 * File:    nectar.c
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#include "nectar.h"

nectar_t nectar =
{
    .state      = NECTAR_NORMAL,
    .init       = nectar_init,
    .transmit   = nectar_transmit,
    .receive    = nectar_receive,
    .compile    = nectar_compile,
    .status     = nectar_status,
};

inline void nectar_init(void)
{
    // attempt to connect with each device, if device is connected
    // the state variable for each device will be set to DEVICE_CONNECTED
    // this is used to compute the bitmask for our subpayloads
    // acknowledge the fact that the bitmask is wasting space within subpayload
    // since we are not currently supporting hot-swapping of sensors
    devices_init();
    LED_On(LED0);
}

uint8_t buffer[] = "hello world";

// we should pop from our queue of payloads only when xbee.tx_state == SERIAL_IDLE
inline void nectar_transmit(void)
{
    // TODO: if queue is empty, return;

    if (xbee.tx_state == SERIAL_IDLE)
    {
        // pop from subpayload queue
        // set transmit request with appropriate data
        LOG(DEBUG_LEVEL, "[NECTAR] setting transmit request with new data.");
        set_transmit_request(buffer, sizeof(buffer)-1, &xbee.api_frame);
    }

    xbee.transmit();
}

inline void nectar_receive(void)
{
    // todo: collect data from sensors
//  LOG(DEBUG_LEVEL, "xbee rx-state: %u", xbee.rx_state);
    xbee.receive();
    //coz_ir.receive();
}

void nectar_compile(void)
{
    // format data from sensors into subpayloads, then payloads
}

// flash LED based on the current state of nectar
void nectar_status(void)
{
    switch(nectar.state)
    {
        // 2 quick flashes after cycle
        case NECTAR_NORMAL:
            for (uint8_t i = 0; i < 4; ++i)
            {
                LED_Toggle(LED0);
                delay_ms(50);
            }
            break;

        // 4 slightly slower flashes
        case NECTAR_WARNING:
            for (uint8_t i = 0; i < 8; ++i)
            {
                LED_Toggle(LED0);
                delay_ms(500);
            }
            break;

        // slow flashes indicate error
        case NECTAR_ERROR:
            while(1)
            {
                LED_Toggle(LED0);
                delay_s(2);
            }
    }
}
