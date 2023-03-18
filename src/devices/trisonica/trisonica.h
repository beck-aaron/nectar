/*******************************************************************************
 * File:    trisonica.h
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#ifndef _TRISONICA_H_
#define _TRISONICA_H_

#include <vector.h>

#define TRISONICA_MAX_TX  256  // TODO: determine how much memory our
#define TRISONICA_MAX_RX  256  // program has access to... fix these values
#define trisonica_receive TRISONICA_UART_HANDLER

typedef enum
{
    TRISONICA_IDLE,
    TRISONICA_PENDING,

} trisonica_state_t;

typedef struct
{
    // specific data types go here
    vector_t tx_buffer;
    vector_t rx_buffer;
    trisonica_state_t tx_state;
    trisonica_state_t rx_state;

    void (*encode)(void);
    void (*decode)(void);
    void (*transmit)(void);
    void (*receive)(void);
    void (*configure)(void);

} trisonica_t;
extern trisonica_t trisonica;

void trisonica_encode(void);
void trisonica_decode(void);
void trisonica_transmit(void);
void trisonica_receive(void);
void trisonica_configure(void);


#endif /* _TELAIRE_H_ */

