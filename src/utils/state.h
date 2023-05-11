#ifndef _STATE_H_
#define _STATE_H_

typedef enum
{
    DEVICE_CONNECTED,
    DEVICE_DISCONNECTED,

} device_state_t;

typedef enum
{
    SERIAL_IDLE,
    SERIAL_PENDING,

} serial_state_t;


#endif /* _STATE_H_ */
