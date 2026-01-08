#ifndef JSON_SERIALIZER_H
#define JSON_SERIALIZER_H

#include <stddef.h>

/* -------- Data structures -------- */

typedef struct {
    const char *timestamp;
    const char *meter_datetime;
    float       total_m3;
    const char *status;
} DataPoint;

typedef struct {
    const char *media;
    const char *meter;
    const char *deviceId;
    const char *unit;
    DataPoint  *data;
    size_t      data_count;
} DeviceReading;

typedef struct {
    size_t          device_count;
    DeviceReading  *readings;
} ValuesBlock;

typedef struct {
    const char *gatewayId;
    const char *date;
    const char *deviceType;
    int         interval_minutes;
    int         total_readings;
    ValuesBlock values;
} GatewayPayload;

/* -------- Error codes -------- */

typedef enum {
    JSON_OK = 0,
    JSON_ERR_BUFFER_TOO_SMALL,
    JSON_ERR_INVALID_ARGUMENT
} json_status_t;

/* -------- API -------- */

json_status_t serialize_to_json(
        const GatewayPayload *payload,
        char *out_buf,
        size_t buf_size,
        size_t *written_len);

#endif
