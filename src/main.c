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
#include <devices.h>
#include <logger.h>

xbee_t xbee;

int main(void)
{
    sysclk_init();
    board_init();

    // initialize all external devices
    devices_init();

    // signal initialization is complete
    LED_On(LED0);

    while(1)
    {   // main loop
        LED_Toggle(LED0);
        delay_ms(500);   
    }
}
