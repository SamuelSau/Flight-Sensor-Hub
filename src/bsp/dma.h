#ifndef BSP_DMA_H
#define BSP_DMA_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t g_adc_dma_sem;

void dma_init(uint16_t *dest_buf, uint16_t num_channels);

#endif /* BSP_DMA_H */
