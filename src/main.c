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

    // wrapper to object-like sensors
    //sensor_t trisonica_mini = {tsm_init, tsm_read};
    //sensor_t telaire = {telaire_init, telaire_read};
    sensor_t coz_ir = {coz_ir_init, coz_ir_read};

    //trisonica_mini.init();
    //telaire.init();
    coz_ir.init();

    LOG(DEBUG_LEVEL, "initialized all peripherals, starting main loop.");

    while(1)
    {   // main loop
        LED_Toggle(LED0);
        coz_ir.read();
        delay_ms(500);   
    }
}
