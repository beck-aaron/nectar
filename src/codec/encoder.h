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
#include "../util/types.h"

#define BITMASK \
    NECTAR_TIMESTAMP | NECTAR_CO2_PPM

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

/**
 * @brief 
 */
void encoder_init(void);

/**
 * @brief Takes a buffer and xbee frame data to format an api payload
 *
 * @return 
 */
void encode_frame(uint8_t* buffer);

/**
 * @brief Formats a nectar payload
 *
 * @return 
 */
void encode_payload(payload_t payload, uint8_t* buffer);

/**
 * @brief Formats a nectar subpayload
 *
 * @return 
 */
void encode_subpayload(subpayload_t subpayload, byte_vector_t* buffer);

#endif // _ENCODER_H_
