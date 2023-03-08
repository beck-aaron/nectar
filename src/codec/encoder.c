/*******************************************************************************
 * File:    encoder.c
 * Author:  Aaron Beck
 *
 * Common codec for the nectar protocol.  Functionality includes encoding
 * subpayloads, payloads, and xbee api commands.
 *
 ******************************************************************************/
#include "encoder.h"

void encode_frame(uint8_t* buffer)
{
}

void encode_payload(payload_t payload, uint8_t* buffer)
{
}

// bytevector... create wrapper for buffer, will handle pushing any data type

void encode_subpayload(subpayload_t subpayload, byte_vector_t* buffer)
{
    /*
    if((BITMASK & TIMESTAMP)    != 0) ;
    //if((BITMASK & CO2_PPM)      != 0) push_uint16(buffer, subpayload.co2_ppm);
    //if((BITMASK & U_VECTOR)     != 0) push_float(buffer, subpayload.u_vector);
    //if((BITMASK & V_VECTOR)     != 0) push_float(buffer, subpayload.v_vector);
    //if((BITMASK & W_VECTOR)     != 0) push_float(buffer, subpayload.w_vector);
    //if((BITMASK & TEMPERATURE)  != 0) push_float(buffer, subpayload.temperature);
    //if((BITMASK & HUMIDITY)     != 0) push_float(buffer, subpayload.humidity);
    //if((BITMASK & PRESSURE)     != 0) push_float(buffer, subpayload.pressure);

    push(buffer, 
    push(buffer, subpayload.timestamp, FLOAT);
    push(buffer, subpayload.timestamp, UINT8);
    push(buffer, subpayload.timestamp, UINT8);
    */
}
