#include "vector.h"

void v_set(const uint8_t* byte, size_t size)
{
    vector.data = (uint8_t*)byte;
    vector.limit = size;
    vector.size = 0;
}

// little endian push
void v_push(const void* byte, size_t size)
{
    for (size_t i = size; i > 0; --i)
    {
        if (vector.size > vector.limit)
            return;

        vector.data[vector.size++] = ((uint8_t*)byte)[i-1];
    }
}

//little endian push function, used for c-strings and any byte-sized array of memory
//only works for byte-sized arrays!!!
//to make it work for multiple data type arrays, need to define the size of one
//element and use the prior push function for each element.
void v_push_bytes(const void* byte, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (vector.size > vector.limit)
            return;

        vector.data[vector.size++] = ((uint8_t*)byte)[i];
    }
}

void v_clear(void)
{
    memset(vector.data, 0, vector.size);
    vector.size = 0;
}
