#include "telemetry_task.h"
#include "processing_task.h"
#include "sensor_task.h"
#include "flight_data.h"
#include "uart.h"
#include "task.h"
#include <string.h>

/*
 * CRC-16/CCITT-FALSE:
 *   Polynomial: 0x1021
 *   Init: 0xFFFF
 *   No final XOR
 */
static uint16_t crc16_ccitt(const uint8_t *data, size_t len)
{
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

void telemetry_task(void *pvParameters)
{
    (void)pvParameters;

    uint32_t sequence = 0;
    FlightTelemetry telem;

    for (;;) {
        /* Block until processed data is available */
        if (xQueueReceive(g_processed_data_queue, &telem, portMAX_DELAY) == pdTRUE) {
            TelemetryPacket pkt;

            pkt.sync_word     = TELEMETRY_SYNC_WORD;
            pkt.version       = TELEMETRY_VERSION;
            pkt.reserved      = 0;
            pkt.sequence      = sequence++;
            pkt.timestamp     = telem.timestamp;
            pkt.altitude_m    = telem.altitude_m;
            pkt.accel_x_g     = telem.accel_x_g;
            pkt.accel_y_g     = telem.accel_y_g;
            pkt.accel_z_g     = telem.accel_z_g;

            pkt.raw_altimeter = telem.raw.altimeter;
            pkt.raw_accel_x   = telem.raw.accel_x;
            pkt.raw_accel_y   = telem.raw.accel_y;
            pkt.raw_accel_z   = telem.raw.accel_z;

            /* Calculate CRC over everything except the CRC field itself */
            pkt.crc16 = crc16_ccitt((const uint8_t *)&pkt,
                                     sizeof(TelemetryPacket) - sizeof(uint16_t));

            /* Transmit the complete packet */
            uart_send((const uint8_t *)&pkt, sizeof(TelemetryPacket));
        }
    }
}
