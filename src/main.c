/*******************************************************************************
 * File:    trisonica-mini.c
 * Author:  Aaron Beck
 *
 * Drives the trisonica-mini sensor, attached to a UART interface defined by
 * config/conf_uart_serial.h
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
