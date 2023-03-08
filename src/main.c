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

int main(void)
{
    sysclk_init();  // necessary for program startup
    board_init();   // necessary to enable peripherals

    logger_init();
    //tsm_init();
    //telaire_init();
    //coz_ir_init();

    LOG(DEBUG_LEVEL, "initialized all peripherals, starting main loop.");

    while(1)
    {   // main loop
        LED_Toggle(LED0);
        delay_ms(500);   
        //telaire_read();
        //tsm_read();
        //coz_ir_read();
    }
}
