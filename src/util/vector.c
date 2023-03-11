#include "vector.h"
#include "logger.h"

vector_t* vector_init(size_t size)
{
    vector_t* vector = malloc(sizeof(*vector));
    vector->data = calloc(sizeof(uint8_t), size);
    vector->size = 0;
    vector->limit = size;
    return  vector;
}

vector_t* vector_set(uint8_t* byte, size_t size)
{
    vector_t* vector = malloc(sizeof(*vector));
    vector->data = byte;
    vector->size = 0;
    vector->limit = size;
    return  vector;
}

// little endian push
void vector_push(const void* byte, size_t size, vector_t* vector)
{
    for (size_t i = size; i > 0; --i)
    {
        if (vector->size > vector->limit)
            return; // reallocate data

        vector->data[vector->size++] = ((uint8_t*)byte)[i-1];
    }
}

//little endian push function, used for c-strings and any byte-sized array of memory
//only works for byte-sized arrays!!!
//to make it work for multiple data type arrays, need to define the size of one
//element and use the prior push function for each element.
void vector_push_bytes(const void* byte, size_t size, vector_t* vector)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (vector->size > vector->limit)
            return;

        vector->data[vector->size++] = ((uint8_t*)byte)[i];
    }
}

void vector_clear(vector_t* vector)
{
    memset(vector->data, 0, vector->size);
    vector->size = 0;
}

void vector_print(vector_t* vector)
{
    LOG(DEBUG_LEVEL, "vector size: %u", vector->size);
    LOGHEX(DEBUG_LEVEL, "vector dump", vector->data, vector->limit);
}

void vector_destroy(vector_t* vector)
{
    free(vector);
}
