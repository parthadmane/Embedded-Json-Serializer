#include <stdio.h>
#include "json_serializer.h"

int main(void)
{
    DataPoint dp = {
        "1970-01-01 00:00",
        "1970-01-01 00:00",
        107.752f,
        "OK"
    };

    DeviceReading dr = {
        "water",
        "waterstarm",
        "stromleser_50898527",
        "m3",
        &dp,
        1
    };

    ValuesBlock vb = {
        1,
        &dr
    };

    GatewayPayload payload = {
        "gateway_1234",
        "1970-01-01",
        "stromleser",
        15,
        1,
        vb
    };

    char buffer[1024];
    size_t written;

    json_status_t res =
        serialize_to_json(&payload, buffer, sizeof(buffer), &written);

    if (res == JSON_OK) {
        printf("Generated JSON:\n%s\n", buffer);
    } else {
        printf("Error: buffer too small!\n");
    }

    return 0;
}
