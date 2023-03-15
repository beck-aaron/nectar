#include "nectar.h"

static void nectar_init(void);
static void nectar_transmit(void);
static void nectar_receive(void);
static void nectar_collect(void);

xbee_t xbee;

nectar_t nectar =
{
    .init       = nectar_init,
    .transmit   = nectar_transmit,
    .receive    = nectar_receive,
    .collect    = nectar_collect,
};

static void nectar_init(void)
{
    devices_init();
    LED_On(LED0);
//  xbee.test();
}

static void nectar_transmit(void)
{
    // TODO
    // check if conditions are right for sending a transmission
    // if they are, send the transmission
}

static void nectar_receive(void)
{
    // TODO
    // check if conditions are right for processing a received transmission
    //xbee.force_receive();
//  xbee.receive();
}

static void nectar_collect(void)
{
    // TODO
    // check if data is ready from sensors, if it is, format into a
    // subpayload and add to queue
}
