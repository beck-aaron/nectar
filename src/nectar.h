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

typedef struct nectar_subpayload_t nectar_subpayload_t;
struct nectar_subpayload_t
{
    size_t      size;
    uint16_t    bitmask;
    time_t      timestamp;
    uint16_t    co2_ppm;
    float       u_vector;
    float       v_vector;
    float       w_vector;
    float       temperature;
    float       humidity;
    float       pressure;

    nectar_subpayload_t* next;
};

typedef struct
{
    uint8_t count;
    size_t  size;
    nectar_subpayload_t* list;

} nectar_payload_t;

typedef struct
{
    size_t max_payload_size; //todo: get this value from the xbee during init.
    uint16_t device_bitmask; // set after initialization, used to determine what
                             // devices we should check for data

    // nectar usees these devices
    xbee_t xbee;
    coz_ir_t coz_ir;
    telaire_t telaire;
    trisonica_t trisonica;

} nectar_t;

/**
 * @brief 
 */
void nectar_init(nectar_t* nectar);

/**
 * @brief 
 */
void nectar_transmit(nectar_t* nectar);

/**
 * @brief 
 */
void nectar_receive(nectar_t* nectar);

/**
 * @brief 
 */
void nectar_compile(nectar_t* nectar);

#endif /* _NECTAR_H_ */
