/*******************************************************************************
 * File:    nectar.h
 * Author:  Aaron Beck
 *
 * Description: Entry point for the nectar driver. Main functions include transmit,
 * receive, and compile.  Transmit communicates with the xbee, sending the currently
 * encoded transmit request if ready.  Receive checks the xbee dma transfer to
 * detemine if a full xbee frame has been sent, and decodes that frame once it is sent.
 * Compile polls sensors for data, currently just the COZ-IR sensor.
 *
 * A payload queue stores compiled data when the network is down or disconnected.
 * As soon as the network connection is restored, data will be flushed out of the queue
 * and regular transmission will occur.
 *
 ******************************************************************************/
#ifndef _NECTAR_H_
#define _NECTAR_H_

#include <asf.h>
#include <clock.h>
#include <logger.h>
#include <devices.h>
#include <queue.h>

#define MAX_SUBPAYLOADS 0x0D    // computed based off of smallest subpayload
#define MAX_PAYLOAD_SIZE 0x54   // determined based on XBee specification
#define MAX_PAYLOADS 0x04       // TODO: determine how much memory we can use for this...

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
    uint32_t    index;
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
    bool        full;
    size_t      size;
    uint8_t     delimiter;          // delimiter is always 0xAF
    uint16_t    datapoints;         // bitmask of current data points being recorded - see user manual
    uint8_t     subpayload_count;   // number of subpayloads in a current payload
    size_t      subpayload_size;    // size (in bytes) of payload

    nectar_subpayload_t subpayloads[MAX_SUBPAYLOADS];

} nectar_payload_t;

typedef struct
{
    // driver usees these devices
    xbee_t xbee;
    coz_ir_t coz_ir;
    telaire_t telaire;              // TODO
    trisonica_t trisonica;          // TODO

    vector_t encoded_buffer;        // stores currently encoded payload
    uint32_t subpayload_index;      // stores unique index for subpayload

    // used to store recorded payloads
    queue_t payload_queue;
    nectar_payload_t payload_buffer[1];

} nectar_t;

/**
 * Initializes logger, dma, sensor drivers, and all local variables
 *
 * @param nectar - the nectar struct to initialize
 */
void nectar_init(nectar_t* nectar);

/**
 * Transmits the front of the payload queue to the basestation, if xbee is ready
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
