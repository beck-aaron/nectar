#include "types.h"

inline void push_byte(byte_vector_t* this, uint8_t byte)
{
    if (this->index < this->limit)
    {
        this->data[this->index++] = byte;
        this->size++;
    }
}

inline void push(byte_vector_t* this, void* buffer, size_t size)
{
    uint8_t* data = buffer;

    for (size_t i = 0; i < size; ++i)
    {
        push_byte(this, (*data >> (4*i) & 0xFF));
    }
}

inline byte_vector_t byte_vector(uint8_t* data, size_t size)
{
    return (byte_vector_t)
    {
        .index  = 0,
        .data   = data,
        .size   = 0,
        .limit  = size
    };
}
