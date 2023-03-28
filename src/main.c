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

int main(void)
{
    sysclk_init();
    board_init();

    nectar_t nectar_data;
    nectar_init(&nectar_data);

    while(true)
    {
        nectar_transmit(&nectar_data);
        nectar_receive(&nectar_data);
        nectar_compile(&nectar_data);
    }
}

