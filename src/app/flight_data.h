#ifndef APP_FLIGHT_DATA_H
#define APP_FLIGHT_DATA_H

#include <stdint.h>

#define TELEMETRY_SYNC_WORD  0xAA55U
#define TELEMETRY_VERSION    1U

/* Raw ADC readings from 4 channels */
typedef struct {
    uint16_t altimeter;   /* ADC1_IN0 - barometric pressure */
    uint16_t accel_x;     /* ADC1_IN1 */
    uint16_t accel_y;     /* ADC1_IN4 */
    uint16_t accel_z;     /* ADC1_IN5 */
} RawSensorData;

/* Processed flight telemetry values (carries raw data forward for packet) */
typedef struct {
    float altitude_m;     /* Altitude in meters */
    float accel_x_g;      /* X acceleration in g */
    float accel_y_g;      /* Y acceleration in g */
    float accel_z_g;      /* Z acceleration in g */
    uint32_t timestamp;   /* Tick count at sampling time */
    RawSensorData raw;    /* Original ADC readings */
} FlightTelemetry;

/*
 * Binary telemetry packet format (38 bytes total):
 *
 * Offset  Size  Field
 * ------  ----  -----
 *   0      2    sync_word (0xAA55)
 *   2      1    version
 *   3      1    reserved
 *   4      4    sequence
 *   8      4    timestamp
 *  12      4    altitude_m (float)
 *  16      4    accel_x_g (float)
 *  20      4    accel_y_g (float)
 *  24      4    accel_z_g (float)
 *  28      2    raw_altimeter
 *  30      2    raw_accel_x
 *  32      2    raw_accel_y
 *  34      2    raw_accel_z
 *  36      2    crc16
 */
typedef struct __attribute__((packed)) {
    uint16_t sync_word;
    uint8_t  version;
    uint8_t  reserved;
    uint32_t sequence;
    uint32_t timestamp;
    float    altitude_m;
    float    accel_x_g;
    float    accel_y_g;
    float    accel_z_g;
    uint16_t raw_altimeter;
    uint16_t raw_accel_x;
    uint16_t raw_accel_y;
    uint16_t raw_accel_z;
    uint16_t crc16;
} TelemetryPacket;

#endif /* APP_FLIGHT_DATA_H */
