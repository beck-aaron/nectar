/*
 *
 * API frame definitions for xbee
 * Generic frame definitions, to be used for encoding api frame
 *
 * note: ignoring xdmac for rapid prototyping, plan to integrate this later
 *
 *
 * API FRAME STRUCTURE
 *  ----------------------------------------------------
 * | START DELIMITER |  LENGTH  | FRAME DATA | CHECKSUM |
 * |-----------------|----------|------------|----------|
 * |       1 byte    |  2 bytes |  n bytes   |  1 byte  |
 *  ----------------------------------------------------
 *
 * FRAME DATA
 *  -------------------------------------------
 * | API identifier | Identifier-specific data |
 * |----------------|--------------------------|
 * | cmdID (1-byte) |    cmdData  (n-bytes)    |
 *  -------------------------------------------
 *
 */
#ifndef _API_FRAMES_H_
#define _API_FRAMES_H_

#include <stdint.h>
#include <stdlib.h>
#include <vector.h>

#define API_FRAME_DELIMITER                 0x7E
#define API_FRAME_HEADER_IDX                0x1
#define API_FRAME_HEADER_LENGTH             0x3

#define TRANSMIT_REQUEST_64_BIT_ADDRESS     0x0
#define TRANSMIT_REQUEST_16_BIT_ADDRESS     0xFFFE
#define TRANSMIT_REQUEST_NO_BROADCAST       0x0
#define TRANSMIT_REQUEST_TX_OPTIONS         0x0

typedef enum
{
    DELIMITER   = 0x7E,
    ESCAPE      = 0x7D,
    XON         = 0x11,
    XOFF        = 0x13

} esc_char_t;

typedef enum
{
    AT_COMMAND                                  = 0x08,
    AT_COMMAND_QUEUE_PARAM                      = 0x09,
    ZIGBEE_TRANSMIT_REQUEST                     = 0x10,
    EXPLICIT_ADDRESSING_ZIGBEE_COMMAND_FRAME    = 0x11,
    REMOTE_COMMAND_REQUEST                      = 0x17,
    CREATE_SOURCE_ROUTE                         = 0x21,
    AT_COMMAND_RESPONSE                         = 0x88,
    MODEM_STATUS                                = 0x8A,
    ZIGBEE_TRANSMIT_STATUS                      = 0x8B,
    ZIGBEE_RECEIVE_PACKET                       = 0x90,
    ZIGBEE_EXPLICIT_RX_INDICATOR                = 0x91,
    ZIGBEE_IO_DATA_SAMPLE_RX_INDICATOR          = 0x92,
    XBEE_SENSOR_READ_INDICATOR                  = 0x94,
    NODE_IDENTIFICATION_INDICATOR               = 0x95,
    REMOTE_COMMAND_RESPONSE                     = 0x97,
    EXTENDED_MODEM_STATUS                       = 0x98,
    OVER_THE_AIR_FIRMWARE_UPDATE_STATUS         = 0xA0,
    ROUTE_RECORD_INDICATOR                      = 0xA1,
    MANY_TO_ONE_ROUTE_REQUEST_INDICATOR         = 0xA3,

} api_frame_type;

typedef enum 
{
    SUCCESS                             = 0x00,
    MAC_ACK_FAILURE                     = 0x01,
    CCA_LBT_FAILURE                     = 0x02,
    NO_SPECTRUM_FREE                    = 0x03,
    INVALID_DEST_ENDPOINT               = 0x15,
    NETWORK_ACK_FAILURE                 = 0x21,
    NOT_JOINED_TO_NETWORK               = 0x22,
    SELF_ADDRESSED                      = 0x23,
    ADDRESS_NOT_FOUND                   = 0x24,
    ROUTE_NOT_FOUND                     = 0x25,
    BROADCAST_SOURCE_FAILED             = 0x26,
    INVALID_BINDING_TABLE               = 0x2B,
    RESOURCE_ERROR_ONE                  = 0x2C,
    ATTEMPTED_BROADCAST_WITH_APS_TX     = 0x2D,
    ATTEMPTED_UNICAST_WITH_APS_TX       = 0x2E,
    INTERNAL_RESOURCE_ERROR             = 0x31,
    RESOURCE_ERROR_TWO                  = 0x32,
    NO_SECURE_CONNECTION                = 0x34,
    ENCRYPTION_FAILURE                  = 0x35,
    DATA_PAYLOAD_TOO_LARGE              = 0x74,
    INDIRECT_MSG_UNREQUESTED            = 0x75,

} delivery_status_t;

typedef enum
{
    NO_DISCOVERY_OVERHEAD               = 0x00,
    ZIGBEE_ADDRESS_DISCOVERY            = 0x01,
    ROUTE_DISCOVERY                     = 0x02,
    ZIGBEE_ADDRESS_AND_ROUTE_DISCOVERY  = 0x03,
    ZIGBEE_END_DEVICE_EXTENDED_TIMEOUT  = 0x40,

} discovery_status_t;

/**
 * @brief TODO
 */
typedef struct
{
    uint8_t  frame_id;
    uint16_t code;
    uint16_t param;

} at_command_t;

/**
 * @brief TODO
 */
typedef struct
{
    uint8_t  frame_id;
    uint64_t destination_address_64;
    uint16_t destination_address_16;
    uint8_t  broadcast_radius;
    uint8_t  transmit_options;
    uint8_t* payload;
    size_t   size;

} transmit_request_t;

/**
 * @brief  TODO
 */
typedef struct
{
    uint8_t             frame_id;
    uint16_t            destination_address_16;
    uint8_t             retry_count;
    delivery_status_t   delivery_status;
    discovery_status_t  discovery_status;

} transmit_status_t;

/**
 * @brief TODO
 */
typedef struct
{
    uint8_t cmdID;

    union
    {
        at_command_t        at_command;
        transmit_request_t  transmit_request;
        transmit_status_t   transmit_status;
    };

} api_frame_t;


inline static void set_at_command(void)
{
    // TODO
}

inline static void print_at_command(const at_command_t* at_command)
{
    // TODO
}

/** 
 * Xbee protocol encoding functions.
 * Refer to: XBee/XBee-PRO® S2C Zigbee® RF Module User Guide
 * for more details
 *
 * TODO: determine which commands have parameter values and which do not,
 * only encode parameter when required
 */
inline static void encode_at_command(const at_command_t* at_command, vector_t* vector)
{
    vector_push(&at_command->frame_id, sizeof(uint8_t), vector);
    vector_push(&at_command->code, sizeof(uint16_t), vector);
//  vector_push(&at_command->param, sizeof(uint16_t), xbee.tx_buffer);
}

inline static void decode_at_command(const uint8_t *buffer, const at_command_t *at_command)
{
    // TODO
}

inline static void set_transmit_request(uint8_t* payload, size_t size, api_frame_t* api_frame)
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

inline static void print_transmit_request(const transmit_request_t* transmit_request)
{
    // TODO
}

inline static void encode_transmit_request(const transmit_request_t* transmit_request, vector_t* vector)
{
    vector_push(&transmit_request->frame_id, sizeof(uint8_t), vector);
    vector_push(&transmit_request->destination_address_64, sizeof(uint64_t), vector);
    vector_push(&transmit_request->destination_address_16, sizeof(uint16_t), vector);
    vector_push(&transmit_request->broadcast_radius, sizeof(uint8_t), vector);
    vector_push(&transmit_request->transmit_options, sizeof(uint8_t), vector);
    vector_push_bytes(transmit_request->payload, transmit_request->size, vector);
}

inline static void decode_transmit_request(const uint8_t *buffer, const transmit_request_t *transmit_request)
{
    // TODO
}

#endif /* _API_FRAMES_H_ */
