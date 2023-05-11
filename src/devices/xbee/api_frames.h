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
#include <logger.h>
#include "at_commands.h"

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

#define API_FRAME(X) \
    X(0x08, AT_COMMAND, "AT_COMMAND") \
    X(0x09, AT_COMMAND_QUEUE_PARAM, "AT_COMMAND_QUEUE_PARAM") \
    X(0x10, ZIGBEE_TRANSMIT_REQUEST, "ZIGBEE_TRANSMIT_REQUEST") \
    X(0x11, EXPLICIT_ADDRESSING_ZIGBEE_COMMAND_FRAME, "EXPLICIT_ADDRESSING_ZIGBEE_COMMAND_FRAME") \
    X(0x17, REMOTE_COMMAND_REQUEST, "REMOTE_COMMAND_REQUEST") \
    X(0x21, CREATE_SOURCE_ROUTE, "CREATE_SOURCE_ROUTE") \
    X(0x88, AT_COMMAND_RESPONSE, "AT_COMMAND_RESPONSE") \
    X(0x8A, MODEM_STATUS, "MODEM_STATUS") \
    X(0x8B, ZIGBEE_TRANSMIT_STATUS, "ZIGBEE_TRANSMIT_STATUS") \
    X(0x90, ZIGBEE_RECEIVE_PACKET, "ZIGBEE_RECEIVE_PACKET") \
    X(0x91, ZIGBEE_EXPLICIT_RX_INDICATOR, "ZIGBEE_EXPLICIT_RX_INDICATOR") \
    X(0x92, ZIGBEE_IO_DATA_SAMPLE_RX_INDICATOR, "ZIGBEE_IO_DATA_SAMPLE_RX_INDICATOR") \
    X(0x94, XBEE_SENSOR_READ_INDICATOR, "XBEE_SENSOR_READ_INDICATOR") \
    X(0x95, NODE_IDENTIFICATION_INDICATOR, "NODE_IDENTIFICATION_INDICATOR") \
    X(0x97, REMOTE_COMMAND_RESPONSE, "REMOTE_COMMAND_RESPONSE") \
    X(0x98, EXTENDED_MODEM_STATUS, "EXTENDED_MODEM_STATUS") \
    X(0xA0, OVER_THE_AIR_FIRMWARE_UPDATE_STATUS, "OVER_THE_AIR_FIRMWARE_UPDATE_STATUS") \
    X(0xA1, ROUTE_RECORD_INDICATOR, "ROUTE_RECORD_INDICATOR") \
    X(0xA3, MANY_TO_ONE_ROUTE_REQUEST_INDICATOR, "MANY_TO_ONE_ROUTE_REQUEST_INDICATOR") \

#define API_FRAME_ENUM(ID, NAME, TEXT) NAME = ID,
#define API_FRAME_TEXT(ID, NAME, TEXT) case ID: return TEXT;

typedef enum {
    API_FRAME(API_FRAME_ENUM)
} api_frame_id;

inline static const char *api_frame_str(int code)
{
    switch (code) {
        API_FRAME(API_FRAME_TEXT)
    }

    return "UNKNOWN API FRAME";
}

#define DELIVERY_STATUS(X) \
    X(0x00, SUCCESS,                            "SUCCESS")                  \
    X(0x01, MAC_ACK_FAILURE,                    "MAC_ACK_FAILURE")          \
    X(0x02, CCA_LBT_FAILURE,                    "CCA_LBT_FAILURE")          \
    X(0x03, NO_SPECTRUM_FREE,                   "NO_SPECTRUM_FREE")         \
    X(0x15, INVALID_DEST_ENDPOINT,              "INVALID_DEST_ENDPOINT")    \
    X(0x21, NETWORK_ACK_FAILURE,                "NETWORK_ACK_FAILURE")      \
    X(0x22, NOT_JOINED_TO_NETWORK,              "NOT_JOINED_TO_NETWORK")    \
    X(0x23, SELF_ADDRESSED,                     "SELF_ADDRESSED")           \
    X(0x24, ADDRESS_NOT_FOUND,                  "ADDRESS_NOT_FOUND")        \
    X(0x25, ROUTE_NOT_FOUND,                    "ROUTE_NOT_FOUND")          \
    X(0x26, BROADCAST_SOURCE_FAILED,            "BROADCAST_SOURCE_FAILED")  \
    X(0x2B, INVALID_BINDING_TABLE,              "INVALID_BINDING_TABLE")    \
    X(0x2C, RESOURCE_ERROR_ONE,                 "RESOURCE_ERROR_ONE")       \
    X(0x2D, ATTEMPTED_BROADCAST_WITH_APS_TX,    "ATTEMPTED_BROADCAST_WITH_APS_TX") \
    X(0x2E, ATTEMPTED_UNICAST_WITH_APS_TX,      "ATTEMPTED_UNICAST_WITH_APS_TX") \
    X(0x31, INTERNAL_RESOURCE_ERROR,            "INTERNAL_RESOURCE_ERROR")  \
    X(0x32, RESOURCE_ERROR_TWO,                 "RESOURCE_ERROR_TWO")       \
    X(0x34, NO_SECURE_CONNECTION,               "NO_SECURE_CONNECTION")     \
    X(0x35, ENCRYPTION_FAILURE,                 "ENCRYPTION_FAILURE")       \
    X(0x74, DATA_PAYLOAD_TOO_LARGE,             "DATA_PAYLOAD_TOO_LARGE")   \
    X(0x75, INDIRECT_MSG_UNREQUESTED,           "INDIRECT_MSG_UNREQUESTED") \

#define DELIVERY_STATUS_ENUM(ID, NAME, TEXT) NAME = ID,
#define DELIVERY_STATUS_TEXT(ID, NAME, TEXT) case ID: return TEXT;

typedef enum {
    DELIVERY_STATUS(DELIVERY_STATUS_ENUM)
} delivery_status_t;

inline static const char *delivery_status_str(int code)
{
    switch (code) {
        DELIVERY_STATUS(DELIVERY_STATUS_TEXT)
    }

    return "UNKNOWN STATUS";
}

#define DISCOVERY_STATUS(X) \
    X(0x00, NO_DISCOVERY_OVERHEAD,              "NO_DISCOVERY_OVERHEAD")        \
    X(0x01, ZIGBEE_ADDRESS_DISCOVERY,           "ZIGBEE_ADDRESS_DISCOVERY")     \
    X(0x02, ROUTE_DISCOVERY,                    "ROUTE_DISCOVERY")              \
    X(0x03, ZIGBEE_END_DEVICE_EXTENDED_TIMEOUT, "ZIGBEE_END_DEVICE_EXTENDED_TIMEOUT") \

#define DISCOVERY_STATUS_ENUM(ID, NAME, TEXT) NAME = ID,
#define DISCOVERY_STATUS_TEXT(ID, NAME, TEXT) case ID: return TEXT;

typedef enum {
    DISCOVERY_STATUS(DISCOVERY_STATUS_ENUM)
} discovery_status_t;

inline static const char *discovery_status_str(int code)
{
    switch (code) {
        DISCOVERY_STATUS(DISCOVERY_STATUS_TEXT)
    }

    return "UNKNOWN STATUS";
}

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


void xbee_set_at_command(void);
void xbee_print_at_command(const at_command_t* at_command);

/** 
 * Xbee protocol encoding functions.
 * Refer to: XBee/XBee-PRO® S2C Zigbee® RF Module User Guide
 * for more details
 *
 * TODO: determine which commands have parameter values and which do not,
 * only encode parameter when required
 */
void xbee_encode_at_command(const at_command_t* at_command, vector_t* vector);
void xbee_decode_at_command(at_command_t* at_command, vector_t* vector);

void xbee_set_transmit_request(uint8_t* payload, size_t size, api_frame_t* api_frame);
void xbee_print_transmit_request(const transmit_request_t* transmit_request);
void xbee_encode_transmit_request(const transmit_request_t* transmit_request, vector_t* vector);
void xbee_decode_transmit_request(transmit_request_t* transmit_request, vector_t* vector);

void xbee_decode_transmit_status(transmit_status_t* transmit_status, vector_t* vector);
#endif /* _API_FRAMES_H_ */
