#include <queue.h>

void queue_init(queue_t* queue, void* buffer, size_t element_size, size_t number_of_elements)
{
    queue->front = (uintptr_t)buffer;
    queue->back = (uintptr_t)buffer;
    queue->size = 0;
    queue->full = false;
    queue->element_size = element_size;
    queue->limit = number_of_elements;
    queue->buffer_head = buffer;
    queue->buffer_tail = (void*)((uintptr_t)buffer + (element_size * number_of_elements));
}

void queue_pop(queue_t* queue)
{
    bool empty_queue = queue->front == queue->back;

    if (queue->front == (uintptr_t)queue->buffer_tail) {
        queue->front = (uintptr_t)queue->buffer_head;
    } else {
        queue->front += queue->element_size;
    }

    if (empty_queue) queue->back = queue->front;

    queue->size--;
    queue->full = false;
}

void queue_push(queue_t* queue)
{
    if (queue->full) return;

    if (queue->back == (uintptr_t)queue->buffer_tail) {
        queue->back = (uintptr_t)queue->buffer_head;
    } else {
        queue->back += queue->element_size;
    }

    queue->size++;
    if (queue->size == queue->limit - 1) {
        queue->full = true;
    }
}

void queue_print(queue_t* queue)
{
    LOG(DEBUG_LEVEL, "[QUEUE] printing queue");
    putchar('|');
    for (size_t i = 0; i < queue->limit; i++) {
        if (queue->front == (uintptr_t)queue->buffer_head + (i * queue->element_size)) {
            printf(" front |");
        } else if (queue->back == (uintptr_t)queue->buffer_head + (i * queue->element_size)) {
            printf("  back |");
        } else {
            printf("   elt |");
        }
    }
}
