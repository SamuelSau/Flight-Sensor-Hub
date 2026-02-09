#include "sensor_task.h"
#include "flight_data.h"
#include "adc.h"
#include "dma.h"
#include "task.h"
#include "semphr.h"

/* DMA destination buffer for ADC scan results */
static uint16_t adc_buf[ADC_NUM_CHANNELS];

QueueHandle_t g_raw_sample_queue = NULL;

void sensor_task(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(10);  /* 100 Hz */

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        /* Trigger single ADC scan via DMA */
        adc_start_scan();

        /* Wait for DMA transfer complete (max 5ms timeout) */
        if (xSemaphoreTake(g_adc_dma_sem, pdMS_TO_TICKS(5)) == pdTRUE) {
            RawSensorData sample;
            sample.altimeter = adc_buf[0];
            sample.accel_x   = adc_buf[1];
            sample.accel_y   = adc_buf[2];
            sample.accel_z   = adc_buf[3];

            /* Non-blocking send - drop if queue full */
            xQueueSend(g_raw_sample_queue, &sample, 0);
        }
    }
}

uint16_t *sensor_task_get_adc_buf(void)
{
    return adc_buf;
}
