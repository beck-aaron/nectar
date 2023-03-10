#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct vector_t vector_t;
struct vector_t
{
    uint8_t*    data;
    size_t      size;
    size_t      limit;
    void (*push)(vector_t* this, const void* byte, size_t size);
    void (*push_bytes)(vector_t* this, const void* byte, size_t size);
//  void (*push_array)(vector_t* this, const void* byte, size_t element_size, size_t size);
    void (*clear)(vector_t* this);
    void (*destroy)(vector_t* this);
};

vector_t vector(size_t size);

#endif /* _BYTE_VECTOR_H_ */
