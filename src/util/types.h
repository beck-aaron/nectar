#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>
#include <stdlib.h>

/*
#define type_union(VAR_TYPE, ARRAY_TYPE, NAME)  \
                                                \
    typedef union                               \
    {                                           \
        VAR_TYPE variable;                      \
        ARRAY_TYPE data[sizeof(VAR_TYPE)];      \
    } NAME;
*/

typedef enum
{
   UINT8, UINT16, UINT32, UINT64, FLOAT

} type_t;

typedef struct
{
    uint8_t     index;
    uint8_t*    data;
    size_t      size;
    size_t      limit;

} byte_vector_t;

/*
type_union(float, uint8_t, float_u);
type_union(uint16_t, uint8_t, uint16_u);
type_union(uint32_t, uint8_t, uint32_u);
type_union(uint64_t, uint8_t, uint64_u);
*/

#endif /* _TYPES_H_ */
