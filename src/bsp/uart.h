#ifndef BSP_UART_H
#define BSP_UART_H

#include <stdint.h>
#include <stddef.h>

void uart_init(void);
void uart_send(const uint8_t *data, size_t len);
void uart_send_byte(uint8_t byte);

#endif /* BSP_UART_H */
