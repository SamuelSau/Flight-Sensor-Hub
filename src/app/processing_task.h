#ifndef APP_PROCESSING_TASK_H
#define APP_PROCESSING_TASK_H

#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t g_processed_data_queue;

void processing_task(void *pvParameters);

#endif /* APP_PROCESSING_TASK_H */
