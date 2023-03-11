/*******************************************************************************
 * File:    main.c
 * Author:  Aaron Beck
 *
 * Home base for the Nectar driver.  This is where all peripherals will get
 * initialized, sensors will collect data, and the xbee will transmit out
 * important information.
 *
 ******************************************************************************/
#include <asf.h>
#include <logger.h>
#include <devices.h>

xbee_t xbee = XBEE();

int main(void)
{
    sysclk_init();
    board_init();

    // our device initialization
    devices_init();

    LED_On(LED0); // signal initialization is complete

    xbee.test();

    while(1)
    {   // main loop
        LED_Toggle(LED0);
        delay_ms(500);   
    }
}
