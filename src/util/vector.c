#include "vector.h"

// little endian push
static void push(vector_t* this, const void* byte, size_t size)
{
    for (size_t i = size; i > 0; --i)
    {
        if (this->size > this->limit)
            return;

        this->data[this->size++] = ((uint8_t*)byte)[i-1];
    }
}

//little endian push function, used for c-strings and any byte-sized array of memory
//only works for byte-sized arrays!!!
//to make it work for multiple data type arrays, need to define the size of one
//element and use the prior push function for each element.
static void push_bytes(vector_t* this, const void* byte, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (this->size > this->limit)
            return;

        this->data[this->size++] = ((uint8_t*)byte)[i];
    }
}

// TODO: fix function to allow for data of any type
// CURRENTLY BROKEN
/*
static void push_array(vector_t* this, const void* byte, size_t element_size, size_t size)
{
    size_t length = size/element_size;
    const uint8_t* ptr = byte;
    for (size_t i = 0; i < length; ++i)
    {
        if (this->size > this->limit)
            return;

        push(this, &ptr[i], 2);
    }
}
*/

static void clear(vector_t* this)
{
    memset(this->data, 0, this->size);
    this->size = 0;
}

static void destroy(vector_t* this)
{
    free(this->data);
    this->size = 0;
}

vector_t vector(size_t size)
{
    return (vector_t)
    {
        .data       = calloc(size, sizeof(uint8_t)),
        .size       = 0,
        .limit      = size,
        .push       = push,
        .push_bytes = push_bytes,
    //  .push_array = push_array,
        .clear      = clear,
        .destroy    = destroy,
    };
}
