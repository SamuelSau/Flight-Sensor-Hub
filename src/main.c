#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "clock.h"
#include "gpio.h"
#include "uart.h"
#include "adc.h"
#include "dma.h"
#include "flight_data.h"
#include "sensor_task.h"
#include "processing_task.h"
#include "telemetry_task.h"

/* Provided by CMSIS / system */
uint32_t SystemCoreClock = 16000000U;

/* Stack sizes (words) */
#define SENSOR_STACK_SIZE      256
#define PROCESSING_STACK_SIZE  512
#define TELEMETRY_STACK_SIZE   384

int main(void)
{
    /* BSP initialization */
    clock_init();
    gpio_init();
    uart_init();

    /* Create RTOS queues */
    g_raw_sample_queue    = xQueueCreate(8, sizeof(RawSensorData));
    g_processed_data_queue = xQueueCreate(4, sizeof(FlightTelemetry));

    /* Initialize DMA with sensor task's ADC buffer */
    dma_init(sensor_task_get_adc_buf(), ADC_NUM_CHANNELS);

    /* Initialize ADC (after DMA is configured) */
    adc_init();

    /* Create application tasks */
    xTaskCreate(sensor_task,     "Sensor",     SENSOR_STACK_SIZE,     NULL, 6, NULL);
    xTaskCreate(processing_task, "Processing", PROCESSING_STACK_SIZE, NULL, 4, NULL);
    xTaskCreate(telemetry_task,  "Telemetry",  TELEMETRY_STACK_SIZE,  NULL, 3, NULL);

    /* Start the scheduler - should never return */
    vTaskStartScheduler();

    /* Should not reach here */
    for (;;);
}

/* FreeRTOS memory allocation failure hook (optional safety net) */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    for (;;);
}
