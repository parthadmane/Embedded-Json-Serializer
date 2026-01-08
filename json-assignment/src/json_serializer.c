#include "json_serializer.h"
#include <stdio.h>
#include <stdarg.h>

/* helper: safe append */
static int append(char **ptr, size_t *remaining, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(*ptr, *remaining, fmt, args);
    va_end(args);

    if (n < 0 || (size_t)n >= *remaining) return -1;

    *ptr += n;
    *remaining -= n;
    return n;
}

json_status_t serialize_to_json(
        const GatewayPayload *p,
        char *out,
        size_t buf_size,
        size_t *written_len)
{
    if (!p || !out) return JSON_ERR_INVALID_ARGUMENT;

    char *w = out;
    size_t rem = buf_size;

    if (append(&w,&rem,"[\n  {\n") < 0) goto overflow;

    append(&w,&rem,"    \"gatewayId\": \"%s\",\n", p->gatewayId);
    append(&w,&rem,"    \"date\": \"%s\",\n", p->date);
    append(&w,&rem,"    \"deviceType\": \"%s\",\n", p->deviceType);
    append(&w,&rem,"    \"interval_minutes\": %d,\n", p->interval_minutes);
    append(&w,&rem,"    \"total_readings\": %d,\n", p->total_readings);

    append(&w,&rem,"    \"values\": {\n");
    append(&w,&rem,"      \"device_count\": %zu,\n", p->values.device_count);
    append(&w,&rem,"      \"readings\": [\n");

    for (size_t i = 0; i < p->values.device_count; i++) {
        DeviceReading *d = &p->values.readings[i];

        append(&w,&rem,"        {\n");
        append(&w,&rem,"          \"media\": \"%s\",\n", d->media);
        append(&w,&rem,"          \"meter\": \"%s\",\n", d->meter);
        append(&w,&rem,"          \"deviceId\": \"%s\",\n", d->deviceId);
        append(&w,&rem,"          \"unit\": \"%s\",\n", d->unit);
        append(&w,&rem,"          \"data\": [\n");

        for (size_t j = 0; j < d->data_count; j++) {
            DataPoint *dp = &d->data[j];

            append(&w,&rem,"            {\n");
            append(&w,&rem,"              \"timestamp\": \"%s\",\n", dp->timestamp);
            append(&w,&rem,"              \"meter_datetime\": \"%s\",\n", dp->meter_datetime);
            append(&w,&rem,"              \"total_m3\": %.3f,\n", dp->total_m3);
            append(&w,&rem,"              \"status\": \"%s\"\n", dp->status);
            append(&w,&rem,"            }%s\n",
                   (j + 1 < d->data_count) ? "," : "");
        }

        append(&w,&rem,"          ]\n");
        append(&w,&rem,"        }%s\n",
               (i + 1 < p->values.device_count) ? "," : "");
    }

    append(&w,&rem,"      ]\n");
    append(&w,&rem,"    }\n");
    append(&w,&rem,"  }\n]\n");

    if (written_len) *written_len = buf_size - rem;
    return JSON_OK;

overflow:
    return JSON_ERR_BUFFER_TOO_SMALL;
}
