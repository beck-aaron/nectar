/*******************************************************************************
 * File:    encoder.c
 * Author:  Aaron Beck
 *
 * Common codec for the nectar protocol.  Functionality includes encoding
 * subpayloads, payloads, and xbee api commands.
 *
 ******************************************************************************/
#include "encoder.h"
#include "frames.h"

static uint8_t buffer[2048] = {0};

void encoder_reset(void)
{
    memset(buffer, 0, sizeof(buffer));
}

void encode_frame(void)
{
}

// bytevector... create wrapper for buffer, will handle pushing any data type
// encode packet into shared uint8_t buffer
void encode_packet(zigbee_packet_t* packet)
{
}


void encoder_test(void)
{
    // create api_frame first
    // then when packet is created, init with pointer of frame

    at_command_t at_command =
    {
        .frame_id   = 0x1, // determines if a response should be sent
        .command    = ID,  // AT command to send
        .parameter  = 0,   // optional parameter
    };


    /*
    zigbee_packet_t packet = {
        .start_delimiter = FRAME_DELIMITER,
        .length = ,
        .frame = ,
        .checksum = ,
    };

    encode_frame(&packet);
    LOGHEX(DEBUG_LEVEL, "testing encoder for at_command frame", buffer, sizeof(buffer));
    */
}
