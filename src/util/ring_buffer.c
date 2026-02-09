#include "ring_buffer.h"

void ring_buffer_init(RingBuffer *rb, uint8_t *buf, uint16_t size)
{
    rb->buf  = buf;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
}

bool ring_buffer_put(RingBuffer *rb, uint8_t byte)
{
    uint16_t next = (rb->head + 1U) % rb->size;
    if (next == rb->tail) {
        return false;  /* full */
    }
    rb->buf[rb->head] = byte;
    __asm volatile("dmb" ::: "memory");
    rb->head = next;
    return true;
}

bool ring_buffer_get(RingBuffer *rb, uint8_t *byte)
{
    if (rb->head == rb->tail) {
        return false;  /* empty */
    }
    *byte = rb->buf[rb->tail];
    __asm volatile("dmb" ::: "memory");
    rb->tail = (rb->tail + 1U) % rb->size;
    return true;
}

uint16_t ring_buffer_count(const RingBuffer *rb)
{
    int32_t diff = (int32_t)rb->head - (int32_t)rb->tail;
    if (diff < 0) {
        diff += rb->size;
    }
    return (uint16_t)diff;
}

bool ring_buffer_is_empty(const RingBuffer *rb)
{
    return rb->head == rb->tail;
}

bool ring_buffer_is_full(const RingBuffer *rb)
{
    return ((rb->head + 1U) % rb->size) == rb->tail;
}
