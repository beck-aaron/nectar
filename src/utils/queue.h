#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>

// work in progress
typedef struct
{
    void* front;
    void* back;
    size_t size;

} queue_t;

// each element should have a pointer to the next element
// push should insert a data element to the back of the queue
// pop should return a pointer of the element that was first in the queue

#endif /* _QUEUE_H_ */
