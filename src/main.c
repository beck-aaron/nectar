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
    LED_On(LED0);

    nectar.init();

    // main loop

    while(1)
    {
        nectar.transmit();
        nectar.receive();
        LED_Toggle(LED0);
        delay_ms(50);
    }
}

