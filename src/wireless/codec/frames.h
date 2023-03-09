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

#define FRAME_DELIMITER 0x7E

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

} api_frame_name;

/**
 * @brief 
 */
typedef struct
{
    uint8_t  frame_id;
    uint16_t command;
    uint64_t parameter;

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

typedef union
{
    at_command_t        at_command;
    transmit_request_t  transmit_request;

} frame_data_t;

/**
 * @brief 
 */
typedef struct
{
    uint8_t type; 
    frame_data_t data;

} api_frame_t;

typedef struct
{
    uint8_t         start_delimiter;
    uint16_t        length;
    api_frame_t*    frame;
    uint8_t         checksum;

} zigbee_packet_t;


#endif /* _API_FRAMES_H_ */
