#include "processing_task.h"
#include "sensor_task.h"
#include "flight_data.h"
#include "task.h"
#include <math.h>

/*
 * Barometric altitude conversion:
 *
 * Simulated barometric pressure sensor maps ADC range [0, 4095] to
 * pressure range [30 kPa, 110 kPa] (full vacuum to sea-level+).
 *
 * Hypsometric formula (simplified for troposphere):
 *   altitude = 44330 * (1 - (P / P0)^0.1903)
 *   where P0 = 101.325 kPa (sea level)
 *
 * Since we're using hardware FPU (Cortex-M4F), float math is fast.
 */

#define ADC_MAX         4095.0f
#define PRESSURE_MIN    30.0f       /* kPa at ADC=0 */
#define PRESSURE_MAX    110.0f      /* kPa at ADC=4095 */
#define P0_KPA          101.325f    /* Sea-level standard pressure */
#define VREF_MV         3300.0f     /* 3.3V reference */
#define ACCEL_ZERO_MV   1650.0f     /* 0g output (mid-scale) */
#define ACCEL_SENS_MV   330.0f      /* mV per g (typical MEMS) */

QueueHandle_t g_processed_data_queue = NULL;

static float adc_to_altitude(uint16_t raw)
{
    float pressure = PRESSURE_MIN + ((float)raw / ADC_MAX) * (PRESSURE_MAX - PRESSURE_MIN);
    float ratio = pressure / P0_KPA;
    return 44330.0f * (1.0f - powf(ratio, 0.1903f));
}

static float adc_to_accel_g(uint16_t raw)
{
    float voltage_mv = ((float)raw / ADC_MAX) * VREF_MV;
    return (voltage_mv - ACCEL_ZERO_MV) / ACCEL_SENS_MV;
}

void processing_task(void *pvParameters)
{
    (void)pvParameters;

    RawSensorData raw;

    for (;;) {
        /* Block until raw data is available */
        if (xQueueReceive(g_raw_sample_queue, &raw, portMAX_DELAY) == pdTRUE) {
            FlightTelemetry telem;

            telem.altitude_m = adc_to_altitude(raw.altimeter);
            telem.accel_x_g  = adc_to_accel_g(raw.accel_x);
            telem.accel_y_g  = adc_to_accel_g(raw.accel_y);
            telem.accel_z_g  = adc_to_accel_g(raw.accel_z);
            telem.timestamp  = xTaskGetTickCount();
            telem.raw        = raw;

            /* Non-blocking send to telemetry - drop if full */
            xQueueSend(g_processed_data_queue, &telem, 0);
        }
    }
}
