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

    while(true)
    {
        nectar.transmit();
        nectar.receive();
        nectar.compile();
        nectar.status();
    }
}

