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
#include <logger.h>

int main(void)
{
    sysclk_init();
    board_init();

    nectar_t nectar_data;
    nectar_init(&nectar_data);

    delay_s(5);

    /*
     * testing queue functions
     */
    {
        LOG(DEBUG_LEVEL, "starting queue test");
        nectar_payload_t buffer[5];
        queue_t queue;
        queue_init(&queue, &buffer, sizeof(nectar_payload_t), 5);

        LOG(DEBUG_LEVEL, "testing queue push");
        queue_print(&queue);
        queue_push(&queue);
        queue_print(&queue);

        LOG(DEBUG_LEVEL, "testing queue pop");
        queue_print(&queue);
        queue_pop(&queue);
        queue_print(&queue);

        exit(0);
    }


    while(true)
    {
        nectar_compile(&nectar_data);
        nectar_transmit(&nectar_data);
        nectar_receive(&nectar_data);
    }
}

