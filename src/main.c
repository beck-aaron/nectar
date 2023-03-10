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
#include <sensors.h>
//#include "wireless/codec/encoder.h"

#include "wireless/xbee.h"

int main(void)
{
    sysclk_init();  // necessary for program startup
    board_init();   // necessary to enable peripherals
    LED_On(LED0);

    logger_init();
    xbee_init();

    while(1)
    {   // main loop
        LED_Toggle(LED0);
        delay_ms(500);   
    }
}
