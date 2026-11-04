#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t* rb) {
    rb->head = 0;
    rb->tail = 0;
};

bool ring_buffer_push(ring_buffer_t* rb, uint8_t byte) {
    uint16_t next_head = (rb->head + 1) & (RING_BUFFER_SIZE - 1);

    if (rb->head == rb->tail) {
        return false;
    } else {
        rb->buffer[rb->head] = byte;
        rb->head = next_head;

        return true;
    }
};

bool ring_buffer_pop(ring_buffer_t* rb, uint8_t* byte) {
    if (rb->head == rb->tail) {
        return false;
    } else {
        *byte = rb->buffer[rb->tail];
        rb->tail = (rb->tail + 1) & (RING_BUFFER_SIZE - 1);
        return true;
    }
};

bool ring_buffer_is_empty(const ring_buffer_t *rb) {
    return rb->head == rb->tail;
};

