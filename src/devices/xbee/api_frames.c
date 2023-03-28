#include "api_frames.h"

/** 
 * Xbee protocol encoding functions.
 * Refer to: XBee/XBee-PRO® S2C Zigbee® RF Module User Guide
 * for more details
 *
 * TODO: determine which commands have parameter values and which do not,
 * only encode parameter when required
 */
void xbee_encode_at_command(const at_command_t* at_command, vector_t* vector)
{
    vector_push(&at_command->frame_id, sizeof(uint8_t), vector);
    vector_push(&at_command->code, sizeof(uint16_t), vector);
//  vector_push(&at_command->param, sizeof(uint16_t), xbee.tx_buffer);
}

void xbee_decode_at_command(at_command_t* at_command, vector_t* vector)
{

}

void xbee_set_transmit_request(uint8_t* payload, size_t size, api_frame_t* api_frame)
{
    api_frame->cmdID = ZIGBEE_TRANSMIT_REQUEST;
    transmit_request_t* transmit_request = &api_frame->transmit_request;
    transmit_request->frame_id = 0x1;
    transmit_request->destination_address_64 = TRANSMIT_REQUEST_64_BIT_ADDRESS;
    transmit_request->destination_address_16 = TRANSMIT_REQUEST_16_BIT_ADDRESS;
    transmit_request->broadcast_radius = TRANSMIT_REQUEST_NO_BROADCAST;
    transmit_request->transmit_options = TRANSMIT_REQUEST_TX_OPTIONS;
    transmit_request->payload = payload;
    transmit_request->size = size;
}

void xbee_print_transmit_request(const transmit_request_t* transmit_request)
{
    // TODO
}

void xbee_encode_transmit_request(const transmit_request_t* transmit_request, vector_t* vector)
{
    vector_push(&transmit_request->frame_id, sizeof(uint8_t), vector);
    vector_push(&transmit_request->destination_address_64, sizeof(uint64_t), vector);
    vector_push(&transmit_request->destination_address_16, sizeof(uint16_t), vector);
    vector_push(&transmit_request->broadcast_radius, sizeof(uint8_t), vector);
    vector_push(&transmit_request->transmit_options, sizeof(uint8_t), vector);
    vector_push_bytes(transmit_request->payload, transmit_request->size, vector);

    LOG(ENCODER_LEVEL, "[XBEE] \t%-24s:= %u", "frame id", transmit_request->frame_id);
    LOG(ENCODER_LEVEL, "[XBEE] \t%-24s:= %#0llX", "destination address (64)", transmit_request->destination_address_64);
    LOG(ENCODER_LEVEL, "[XBEE] \t%-24s:= 0x%02X", "destination address (16)", transmit_request->destination_address_16);
    LOG(ENCODER_LEVEL, "[XBEE] \t%-24s:= 0x%02X", "broadcast radius", transmit_request->broadcast_radius);
    LOG(ENCODER_LEVEL, "[XBEE] \t%-24s:= 0x%02X", "transmit options", transmit_request->transmit_options);
    LOGHEX(ENCODER_LEVEL, "[XBEE] \tpayload", transmit_request->payload, transmit_request->size);
}

void xbee_decode_transmit_request(transmit_request_t* transmit_request, vector_t* vector)
{
    // TODO
}

void xbee_decode_transmit_status(transmit_status_t* transmit_status, vector_t* vector)
{

    vector_pop(&transmit_status->frame_id, sizeof(uint8_t), vector);
    vector_pop(&transmit_status->destination_address_16, sizeof(uint16_t), vector);
    vector_pop(&transmit_status->retry_count, sizeof(uint8_t), vector);
    vector_pop(&transmit_status->delivery_status, sizeof(delivery_status_t), vector);
    vector_pop(&transmit_status->discovery_status, sizeof(discovery_status_t), vector);

    LOG(DECODER_LEVEL, "[XBEE] \t%-24s:= %u", "frame id", transmit_status->frame_id);
    LOG(DECODER_LEVEL, "[XBEE] \t%-24s:= %0X", "destination address", transmit_status->destination_address_16);
    LOG(DECODER_LEVEL, "[XBEE] \t%-24s:= %#0X", "retry count", transmit_status->retry_count);
    LOG(DECODER_LEVEL, "[XBEE] \t%-24s:= %s", "delivery status", delivery_status_str(transmit_status->delivery_status));
    LOG(DECODER_LEVEL, "[XBEE] \t%-24s:= %s", "discovery status", discovery_status_str(transmit_status->discovery_status));
}

