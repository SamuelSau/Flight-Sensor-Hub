#ifndef UTIL_RING_BUFFER_H
#define UTIL_RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define UART_TX_BUF_SIZE  512U
#define UART_RX_BUF_SIZE  256U

typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    uint16_t size;
    uint8_t *buf;
} RingBuffer;

void     ring_buffer_init(RingBuffer *rb, uint8_t *buf, uint16_t size);
bool     ring_buffer_put(RingBuffer *rb, uint8_t byte);
bool     ring_buffer_get(RingBuffer *rb, uint8_t *byte);
uint16_t ring_buffer_count(const RingBuffer *rb);
bool     ring_buffer_is_empty(const RingBuffer *rb);
bool     ring_buffer_is_full(const RingBuffer *rb);

#endif /* UTIL_RING_BUFFER_H */
