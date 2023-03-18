/*******************************************************************************
 * File:    nectar.c
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#include "nectar.h"

// declare devices
xbee_t      xbee;
coz_ir_t    coz_ir;
telaire_t   telaire;
trisonica_t trisonica;

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
    devices_init();
    LED_On(LED0);
}

uint8_t buffer[] = "hello world";
inline void nectar_transmit(void)
{
    set_transmit_request(buffer, sizeof(buffer)-1, &xbee.api_frame);
    xbee.transmit();

    // todo: implement transmitting commands to sensors
    // based on data received from xbee
}

inline void nectar_receive(void)
{
    xbee.receive();

    // todo: collect data from sensors
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
