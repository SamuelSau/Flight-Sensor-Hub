#include "uart.h"
#include "ring_buffer.h"
#include "clock.h"
#include "stm32f4xx.h"

static uint8_t tx_buf_mem[UART_TX_BUF_SIZE];
static uint8_t rx_buf_mem[UART_RX_BUF_SIZE];
static RingBuffer tx_rb;
static RingBuffer rx_rb;

void uart_init(void)
{
    ring_buffer_init(&tx_rb, tx_buf_mem, UART_TX_BUF_SIZE);
    ring_buffer_init(&rx_rb, rx_buf_mem, UART_RX_BUF_SIZE);

    /* Enable USART2 clock (on APB1) */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    __asm volatile("dsb");

    /* Disable USART before configuration */
    USART2->CR1 = 0;

    /*
     * Baud rate = fPCLK1 / (8 * (2 - OVER8) * USARTDIV)
     * For 115200 baud with APB1 = 45 MHz, OVER8 = 0:
     *   USARTDIV = 45000000 / (16 * 115200) = 24.4140625
     *   Mantissa = 24, Fraction = 0.4140625 * 16 = 6.625 ≈ 7
     *   BRR = (24 << 4) | 7 = 0x187
     */
    USART2->BRR = (24U << 4) | 7U;

    /* Enable RXNE interrupt */
    USART2->CR1 = USART_CR1_TE       /* Transmitter enable */
                | USART_CR1_RE       /* Receiver enable */
                | USART_CR1_RXNEIE;  /* RXNE interrupt enable */

    /* Set USART2 interrupt priority (6) and enable */
    NVIC_SetPriority(USART2_IRQn, 6);
    NVIC_EnableIRQ(USART2_IRQn);

    /* Enable USART */
    USART2->CR1 |= USART_CR1_UE;
}

void uart_send(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        /* Spin if buffer is full - backpressure */
        while (!ring_buffer_put(&tx_rb, data[i]));
    }
    /* Enable TXE interrupt to start transmission */
    USART2->CR1 |= USART_CR1_TXEIE;
}

void uart_send_byte(uint8_t byte)
{
    while (!ring_buffer_put(&tx_rb, byte));
    USART2->CR1 |= USART_CR1_TXEIE;
}

void USART2_IRQHandler(void)
{
    uint32_t sr = USART2->SR;

    /* TX empty - send next byte from ring buffer */
    if ((sr & USART_SR_TXE) && (USART2->CR1 & USART_CR1_TXEIE)) {
        uint8_t byte;
        if (ring_buffer_get(&tx_rb, &byte)) {
            USART2->DR = byte;
        } else {
            /* No more data - disable TXE interrupt */
            USART2->CR1 &= ~USART_CR1_TXEIE;
        }
    }

    /* RX not empty - store received byte */
    if (sr & USART_SR_RXNE) {
        uint8_t byte = (uint8_t)(USART2->DR & 0xFF);
        ring_buffer_put(&rx_rb, byte);  /* Drop if full */
    }

    /* Clear overrun error if set */
    if (sr & USART_SR_ORE) {
        (void)USART2->DR;
    }
}
