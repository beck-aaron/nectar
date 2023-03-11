#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    uint8_t* data;
    size_t size;
    size_t limit;

} vector_t;

// initialize vector with data
vector_t* vector_init(size_t size);
vector_t* vector_set(uint8_t* byte, size_t size);
void vector_push(const void* byte, size_t size, vector_t* vector);
void vector_push_bytes(const void* byte, size_t size, vector_t* vector);
void vector_clear(vector_t* vector);
void vector_print(vector_t* vector);
void vector_destroy(vector_t* vector);


#endif /* _BYTE_VECTOR_H_ */
