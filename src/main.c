/*******************************************************************************
 * File:    main.c
 * Author:  Aaron Beck
 *
 * Home base for the Nectar driver.  This is where all peripherals will get
 * initialized, sensors will collect data, and the xbee will transmit out
 * important information.
 *
 ******************************************************************************/
#include "nectar.h"

nectar_t nectar;

int main(void)
{
    sysclk_init();
    board_init();

    nectar.init();

    xbee.test();

    while(1)
    {   // main loop

        // fetch data, format into packet, then send
        // sending should be done using dma controller.
        // we should be able to place our payloads in memory
        // and have the dma controller automatically send them.
        LED_Toggle(LED0);

        xbee.receive();

        delay_ms(250);
    }
}
