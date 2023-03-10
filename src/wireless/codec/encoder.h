/*******************************************************************************
 * File:    encoder.h
 * Author:  Aaron Beck
 *
 * Common codec for the nectar protocol.  Functionality includes encoding
 * subpayloads, payloads, and xbee api commands.
 *
 ******************************************************************************/
#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <asf.h>
#include <logger.h>
#include <string.h>
#include <vector.h>
#include "frames.h"
#include "commands.h"

//#include "../wireless/xbee.h"
//#include "commands.h"

#define BITMASK \
    NECTAR_TIMESTAMP | NECTAR_CO2_PPM

#define ZERO 0

// Add future data metrics to this bitmask
enum
{
    TIMESTAMP    = 0b1000000000000000,
    CO2_PPM      = 0b0100000000000000,
    U_VECTOR     = 0b0010000000000000,
    V_VECTOR     = 0b0001000000000000,
    W_VECTOR     = 0b0000100000000000,
    TEMPERATURE  = 0b0000010000000000,
    HUMIDITY     = 0b0000001000000000,
    PRESSURE     = 0b0000000100000000,
};


// Add future data types to this subpayload struct
typedef struct
{
    uint16_t    bitmask;
    uint64_t    timestamp;
    uint16_t    co2_ppm;
    float       u_vector;
    float       v_vector;
    float       w_vector;
    float       temperature;
    float       humidity;
    float       pressure;

} subpayload_t;

typedef struct
{
    uint8_t     count;          // amount of subpayloads within payload
    uint8_t     payload[83];    // array with as many subpayloads as possible
} payload_t;

void encoder_test(void);
void encoder_init(void);
void encoder_destroy(void);
uint8_t* encoder_get(void);
size_t encoder_size(void);

/**
 * @brief 
 */
void encoder_reset(void);

/**
 * @brief Encodes an API frame into the shared encoding buffer
 */
void encode_frame(const api_frame_t* frame);

/**
 * @brief Takes a structure of organized zigbee data to form
 * an encoded buffer of data
 */
void encode_packet(xbee_packet_t* packet);

/**
 * @brief Formats a nectar payload
 */
void encode_payload(payload_t payload, uint8_t* buffer);


/**
 * @brief Escapes all necessary characters in the encoding buffer
 * See 
 */
void escape_characters(void);

/* AT-Command encoding functions */
void encode_at_command(const at_command_t* at_command);
void decode_at_command(const uint8_t* buffer, const at_command_t* at_command);

void encode_transmit_request(const transmit_request_t* transmit_request);
void decode_transmit_request(const uint8_t* buffer, const transmit_request_t* transmit_request);

/**
 * @brief Formats a nectar subpayload
 *
 * @return 
 */
//void encode_subpayload(subpayload_t subpayload, byte_vector_t* buffer);


/*
static void verify_checksum(uint8_t* buffer)
{
}
*/

#endif // _ENCODER_H_
