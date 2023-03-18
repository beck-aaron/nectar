/*******************************************************************************
 * File:    vector.h
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    uint8_t* data;
    uint8_t* iterator;
    volatile size_t size;
    size_t limit;

} vector_t;

inline void vector_init(size_t size, vector_t* vector)
{
    vector->data = calloc(sizeof(uint8_t), size);
    vector->size = 0;
    vector->limit = size;
    vector->iterator = vector->data;
}

inline void vector_set(uint8_t* byte, size_t size, vector_t* vector)
{
    vector->data = byte;
    vector->size = 0;
    vector->limit = size;
    vector->iterator = vector->data;
}

// little endian push
inline void vector_push(const void* byte, size_t size, vector_t* vector)
{
    for (size_t i = size; i > 0; --i)
    {
        if (vector->size > vector->limit)
            return; // reallocate data

        vector->data[vector->size++] = ((uint8_t*)byte)[i-1];
    }
}

// pops a value from the front of the vector, does not clear the data
inline void vector_pop(void* data, size_t size, vector_t* vector)
{
    for (size_t i = size; i > 0; --i)
    {
        if (&vector->data[vector->size] == vector->iterator)
            return;

        ((uint8_t*)data)[i-1] = *vector->iterator++;
    }
}

//little endian push function, used for c-strings and any byte-sized array of memory
//only works for byte-sized arrays!!!
//to make it work for multiple data type arrays, need to define the size of one
//element and use the prior push function for each element.
inline void vector_push_bytes(const void* byte, size_t size, vector_t* vector)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (vector->size > vector->limit)
            return;

        vector->data[vector->size++] = ((uint8_t*)byte)[i];
    }
}

inline void vector_clear(vector_t* vector)
{
    memset(vector->data, 0, vector->size);
    vector->size = 0;
    vector->iterator = vector->data;
}

inline void vector_destroy(vector_t* vector)
{
    free(vector->data);
}

#endif /* _VECTOR_H_ */
