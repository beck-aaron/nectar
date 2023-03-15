#ifndef _TRISONICA_H_
#define _TRISONICA_H_

#include <vector.h>

#define TRISONICA_MAX_TX  256  // TODO: determine how much memory our
#define TRISONICA_MAX_RX  256  // program has access to... fix these values
#define trisonica_receive TRISONICA_UART_HANDLER

void trisonica_encode(void);
void trisonica_decode(void);
void trisonica_transmit(void);
void trisonica_configure(void);
void trisonica_push(void* value, size_t size);

typedef struct
{
    // specific data types go here
    vector_t tx_buffer;
    vector_t rx_buffer;

    void (*encode)(void);
    void (*decode)(void);
    void (*transmit)(void);
    void (*configure)(void);
    void (*push)(void* value, size_t size);

} trisonica_t;
extern trisonica_t trisonica;

#endif /* _TELAIRE_H_ */

