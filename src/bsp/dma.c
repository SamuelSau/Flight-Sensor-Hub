#include "dma.h"
#include "stm32f4xx.h"

SemaphoreHandle_t g_adc_dma_sem = NULL;

void dma_init(uint16_t *dest_buf, uint16_t num_channels)
{
    /* Create binary semaphore for DMA transfer complete signaling */
    g_adc_dma_sem = xSemaphoreCreateBinary();

    /* Enable DMA2 clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    __asm volatile("dsb");

    /* Disable DMA2 Stream0 before configuration */
    DMA2_Stream0->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream0->CR & DMA_SxCR_EN);

    /* Clear all interrupt flags for Stream0 */
    DMA2->LIFCR = DMA_LIFCR_CTCIF0 | DMA_LIFCR_CHTIF0
                | DMA_LIFCR_CTEIF0 | DMA_LIFCR_CDMEIF0
                | DMA_LIFCR_CFEIF0;

    /*
     * DMA2 Stream0 Channel0 configuration (ADC1):
     * - Channel 0 (ADC1)
     * - Peripheral-to-memory
     * - Circular mode disabled (single-scan, re-enabled per trigger)
     * - Peripheral: 16-bit, no increment
     * - Memory: 16-bit, increment
     * - Transfer complete interrupt enabled
     */
    DMA2_Stream0->CR = (0U << DMA_SxCR_CHSEL_Pos)   /* Channel 0 */
                     | DMA_SxCR_MSIZE_0              /* Memory 16-bit */
                     | DMA_SxCR_PSIZE_0              /* Periph 16-bit */
                     | DMA_SxCR_MINC                 /* Memory increment */
                     | DMA_SxCR_TCIE;                /* Transfer complete IRQ */

    /* Number of data items */
    DMA2_Stream0->NDTR = num_channels;

    /* Peripheral address: ADC1 data register */
    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;

    /* Memory address: destination buffer */
    DMA2_Stream0->M0AR = (uint32_t)dest_buf;

    /* Set DMA2_Stream0 interrupt priority (5) and enable */
    NVIC_SetPriority(DMA2_Stream0_IRQn, 5);
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void DMA2_Stream0_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (DMA2->LISR & DMA_LISR_TCIF0) {
        /* Clear transfer complete flag */
        DMA2->LIFCR = DMA_LIFCR_CTCIF0;

        /* Disable stream (single-shot mode) */
        DMA2_Stream0->CR &= ~DMA_SxCR_EN;

        /* Signal sensor task */
        xSemaphoreGiveFromISR(g_adc_dma_sem, &xHigherPriorityTaskWoken);
    }

    /* Clear any error flags */
    DMA2->LIFCR = DMA_LIFCR_CTEIF0 | DMA_LIFCR_CDMEIF0 | DMA_LIFCR_CFEIF0;

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
