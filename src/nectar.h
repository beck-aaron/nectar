/*******************************************************************************
 * File:    nectar.h
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#ifndef _NECTAR_H_
#define _NECTAR_H_

#include <asf.h>
#include <clock.h>
#include <logger.h>
#include <devices.h>

// TODO: test to determine this value
#define MAX_SUBPAYLOADS 2
#define MAX_PAYLOAD_SIZE 0x2A

enum nectar_mask
{
    NECTAR_TIMESTAMP   = 0b1000000000000000,
    NECTAR_CO2_PPM     = 0b0100000000000000,
    NECTAR_U_VECTOR    = 0b0010000000000000,
    NECTAR_V_VECTOR    = 0b0001000000000000,
    NECTAR_W_VECTOR    = 0b0000100000000000,
    NECTAR_TEMPERATURE = 0b0000010000000000,
    NECTAR_HUMIDITY    = 0b0000001000000000,
    NECTAR_PRESSURE    = 0b0000000100000000,
};

typedef struct nectar_subpayload_t nectar_subpayload_t;
struct nectar_subpayload_t
{
    time_t      timestamp;
    uint16_t    co2_ppm;
    float       u_vector;
    float       v_vector;
    float       w_vector;
    float       temperature;
    float       humidity;
    float       pressure;
};

typedef struct
{
    bool full;
    size_t size;
    uint8_t delimiter;
    uint16_t datapoints; // bitmask of current data points being recorded - see user manual
    uint8_t subpayload_count;
    nectar_subpayload_t subpayloads[MAX_SUBPAYLOADS];

} nectar_payload_t;

typedef struct
{
    nectar_payload_t payload;
    size_t max_payload_size; //todo: get this value from the xbee during init.

    // driver usees these devices
    xbee_t xbee;
    coz_ir_t coz_ir;
    telaire_t telaire;
    trisonica_t trisonica;

    // used to store the currently encoded payload
    vector_t payload_buffer;

} nectar_t;

/**
 * @brief 
 *
 * @param nectar
 */
void nectar_init(nectar_t* nectar);

/**
 * @brief 
 *
 * @param nectar
 */
void nectar_transmit(nectar_t* nectar);

/**
 * @brief 
 *
 * @param nectar
 */
void nectar_receive(nectar_t* nectar);

/**
 * @brief 
 *
 * @param nectar
 */
void nectar_compile(nectar_t* nectar);

void nectar_encode_payload(nectar_payload_t* payload, vector_t* buffer);

void nectar_encode_subpayload(nectar_subpayload_t* subpayload, uint16_t datapoints, vector_t* buffer);

/**
 * @brief 
 *
 * @param nectar
 */
void nectar_build(nectar_t* nectar);

#endif /* _NECTAR_H_ */
