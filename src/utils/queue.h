#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <logger.h>

// work in progress
struct queue
{
    uintptr_t buffer_head;
    uintptr_t buffer_tail;
    volatile uintptr_t front;
    volatile uintptr_t back;
    bool full;
    size_t size;
    size_t limit;
    size_t element_size;
};
typedef struct queue queue_t;

void queue_init(queue_t* queue, void* buffer, size_t length, size_t element);
void queue_pop(queue_t* queue);
void queue_push(queue_t* queue);
void queue_print(queue_t* queue);
void queue_flush(queue_t* queue, void (void* callback));

#endif /* _QUEUE_H_ */
