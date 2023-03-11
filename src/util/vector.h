#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void v_set(const uint8_t* byte, size_t size);
void v_push(const void* byte, size_t size);
void v_push_bytes(const void* byte, size_t size);
void v_clear(void);

typedef struct
{
    uint8_t*    data;
    size_t      size;
    size_t      limit;
    void (*set)(const uint8_t* data, size_t size);
    void (*push)(const void* byte, size_t size);
    void (*push_bytes)(const void* byte, size_t size);
    void (*clear)(void);
} vector_t;

extern vector_t vector;

#define VECTOR() {.set = v_set, .push = v_push, .push_bytes = v_push_bytes, .clear = v_clear};

#endif /* _BYTE_VECTOR_H_ */
