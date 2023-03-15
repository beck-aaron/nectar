#ifndef _NECTAR_H_
#define _NECTAR_H_

#include <asf.h>
#include <logger.h>
#include <devices.h>

typedef struct
{
} nectar_payload_t;

typedef struct
{
    uint16_t bitmask;
    time_t timestamp; //TODO: test <time.h> time() function

} nectar_subpayload_t;

// outside interface to use the nectar driver
typedef struct
{
    void (*init)(void);
    void (*collect)(void);
    void (*encode)(void);
    void (*transmit)(void);
    void (*receive)(void);

} nectar_t;
extern nectar_t nectar;

#endif /* _NECTAR_H_ */
