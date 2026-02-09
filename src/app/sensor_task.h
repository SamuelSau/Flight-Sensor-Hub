#ifndef APP_SENSOR_TASK_H
#define APP_SENSOR_TASK_H

#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t g_raw_sample_queue;

void sensor_task(void *pvParameters);
uint16_t *sensor_task_get_adc_buf(void);

#endif /* APP_SENSOR_TASK_H */
