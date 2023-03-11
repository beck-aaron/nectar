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

#include <asf.h>

#define API_FRAME_DELIMITER 0x7E
#define API_FRAME_HEADER_IDX 0x1
#define API_FRAME_HEADER_LENGTH 0x3

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

/**
 * @brief 
 */
typedef struct
{
    uint8_t  frame_id;
    uint16_t code;
    uint16_t param;

} at_command_t;

/**
 * @brief 
 */
typedef struct
{
    uint8_t  frame_id;
    uint64_t destination_address_64;
    uint16_t destination_address_16;
    uint8_t  broadcast_radius;
    uint8_t  transmit_options;
    uint8_t* payload;

} transmit_request_t;

typedef enum 
{
    SUCCESS                         = 0x00,
    MAC_ACK_FAILURE                 = 0x01,
    CCA_LBT_FAILURE                 = 0x02,
    NO_SPECTRUM_FREE                = 0x03,
    INVALID_DEST_ENDPOINT           = 0x15,
    NETWORK_ACK_FAILURE             = 0x21,
    NOT_JOINED_TO_NETWORK           = 0x22,
    SELF_ADDRESSED                  = 0x23,
    ADDRESS_NOT_FOUND               = 0x24,
    ROUTE_NOT_FOUND                 = 0x25,
    BROADCAST_SOURCE_FAILED         = 0x26,
    INVALID_BINDING_TABLE           = 0x2B,
    RESOURCE_ERROR_ONE              = 0x2C,
    ATTEMPTED_BROADCAST_WITH_APS_TX = 0x2D,
    ATTEMPTED_UNICAST_WITH_APS_TX   = 0x2E,
    INTERNAL_RESOURCE_ERROR         = 0x31,
    RESOURCE_ERROR_TWO              = 0x32,
    NO_SECURE_CONNECTION            = 0x34,
    ENCRYPTION_FAILURE              = 0x35,
    DATA_PAYLOAD_TOO_LARGE          = 0x74,
    INDIRECT_MSG_UNREQUESTED        = 0x75,

} delivery_status;

typedef enum
{
    NO_DISCOVERY_OVERHEAD               = 0x00,
    ZIGBEE_ADDRESS_DISCOVERY            = 0x01,
    ROUTE_DISCOVERY                     = 0x02,
    ZIGBEE_ADDRESS_AND_ROUTE_DISCOVERY  = 0x03,
    ZIGBEE_END_DEVICE_EXTENDED_TIMEOUT  = 0x40,

} discovery_status;

/**
 * @brief 
 */
typedef struct
{
    uint8_t         frame_id;
    uint16_t        destination_address_16;
    uint8_t         retry_count;
    delivery_status status;

} extended_transmit_status;


#endif /* _API_FRAMES_H_ */
