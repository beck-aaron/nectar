/*******************************************************************************
 * File:    nectar.h
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#ifndef _NECTAR_H_
#define _NECTAR_H_

#include "stdlib.h"
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

typedef enum
{
    NECTAR_NORMAL,
    NECTAR_WARNING,
    NECTAR_ERROR,

} nectar_state_t;

// outside interface to use the nectar driver
typedef struct
{
    size_t max_payload_size; //todo: get this value from the xbee during init.
    nectar_state_t state;

    void (*init)(void);
    void (*transmit)(void);
    void (*receive)(void);
    void (*compile)(void);
    void (*status)(void);

} nectar_t;
extern nectar_t nectar;

void nectar_init(void);
void nectar_transmit(void);
void nectar_receive(void);
void nectar_compile(void);
void nectar_status(void);


#endif /* _NECTAR_H_ */
