/*******************************************************************************
 * File:    encoder.c
 * Author:  Aaron Beck
 *
 * Common codec for the nectar protocol.  Functionality includes encoding
 * subpayloads, payloads, and xbee api commands.
 *
 ******************************************************************************/
#include "encoder.h"

#define ENCODER_SIZE 256
static vector_t buffer;

void encoder_init(void)
{
    buffer = vector(ENCODER_SIZE);
}

void encoder_destroy(void)
{
    buffer.destroy(&buffer);
}

void encoder_reset(void)
{
    buffer.clear(&buffer);
}

uint8_t* encoder_get(void)
{
    return buffer.data;
}

size_t encoder_size(void)
{
    return buffer.size;
}


/**
 * @brief All encoding functions when implemented should be put here
 *
 * @param frame, the api frame to encode data from
 */
void encode_frame(const api_frame_t* frame)
{
    buffer.push(&buffer, &frame->type, sizeof(frame->type));

    switch(frame->type)
    {
        case AT_COMMAND:
            encode_at_command(&frame->data->at_command);
            break;

        case ZIGBEE_TRANSMIT_REQUEST:
            encode_transmit_request(&frame->data->transmit_request);
            break;
    }
}

/**
 * @brief Calculate the checksum of an API frame
 *
 * Calculate Checksum:
 * 1. Add all bytes of the packet, except the start delimiter 0x7E and length
 * 2. Keep only the lowest 8 bits from the result
 * 3. subtract this quantity from 0xFF
 *
 * Verify Checksum: TODO: move verify explanation to decoder
 * 1. Add all bytes including the checksum; do not include delimiter and length
 * 2. If the checksum is valid, the rightmost byte of the sum equals 0xFF.
 *
 */
static void calculate_checksum(xbee_packet_t* packet)
{
    for (size_t i = FRAME_HEADER_LENGTH; i < buffer.size; ++i)
        packet->checksum += buffer.data[i];

    packet->checksum = 0xFF - packet->checksum;
}

/**
 * @brief Encodes a zigbee packet into the local encoding buffer.
    // push start_delimiter & length
    // encodes frame into buffer
    // when frame is encoded into the buffer, the length is the length
    // of the encoded buffer minus the first 3 bytes;
    // flips endianness here so length gets encoded properly
    // calculate checksum & push checksum
 *
 * @param packet - the zigbee packet data type to be encoded.
 *
 */
void encode_packet(xbee_packet_t* packet)
{
    buffer.push(&buffer, &packet->delimiter, sizeof(packet->delimiter));
    buffer.push(&buffer, &packet->length, sizeof(packet->length));

    encode_frame(packet->frame);
    packet->length = Swap16(buffer.size - FRAME_HEADER_LENGTH);
    memcpy(&buffer.data[FRAME_HEADER_IDX], &packet->length, sizeof(packet->length));

    calculate_checksum(packet);
    buffer.push(&buffer, &packet->checksum, sizeof(packet->checksum));
}

 
// some at_commands have parameters, some don't
// as a hack rn, if the parameter is 0, we will not encode it.
void encode_at_command(const at_command_t* at_command)
{
    buffer.push(&buffer, &at_command->frame_id, sizeof(at_command->frame_id));
    buffer.push(&buffer, &at_command->code, sizeof(at_command->code));
}

void encode_transmit_request(const transmit_request_t* transmit_request)
{
    // TODO
}


/**
 * @brief Test function to use for development of encoder functions
 */
void encoder_test(void)
{
    // create frame data, set frame data within api frame, store api frame
    // within zigbee packet
    frame_data_t frame_data = {.at_command = {0x1, VL, 0}};
    api_frame_t api_frame = {AT_COMMAND, &frame_data};
    xbee_packet_t packet = {FRAME_DELIMITER, ZERO, &api_frame, ZERO};

    // when structs are all populated with data, encode to encoder's buffer
    encoder_init();
    encode_packet(&packet);
    LOG(WARNING_LEVEL, "testing encoder functions");
    LOGHEX(DEBUG_LEVEL, "encoded buffer", buffer.data, buffer.size);
    encoder_destroy();
}
