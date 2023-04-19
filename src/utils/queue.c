#include <queue.h>

void queue_init(queue_t* queue, void* buffer, size_t element_size, size_t number_of_elements)
{
    queue->front = buffer;
    queue->back = NULL;
    queue->size = 0;
    queue->element_size = element_size;
    queue->limit = element_size * number_of_elements;
}

void queue_pop(queue_t* queue)
{
}
