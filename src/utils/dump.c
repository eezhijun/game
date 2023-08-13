#include "stdio.h"
#include "stdint.h"
#include "assert.h"

#define DUMP_BYTES 16
#define DUMP_BUFFER_SIZE 512

static char dump_buffer[DUMP_BUFFER_SIZE] = { 0 };

/**
 * @brief
 *
 * @param data
 * @param len
 */
void dump_x(const uint8_t *data, size_t len)
{
    size_t line = len / DUMP_BYTES;
    int offset = 0;
    for (int i = 0; i < line; i++) {
        if (((int)sizeof(dump_buffer) - offset) < 0) {
            break;
        }

        for (int j = i * DUMP_BYTES; j < DUMP_BYTES + DUMP_BYTES * i; j++) {
            offset += snprintf(dump_buffer + offset,
                               sizeof(dump_buffer) - offset, "%02X ", data[j]);
        }
        offset += snprintf(dump_buffer + offset, sizeof(dump_buffer) - offset, "\n");

    }

    if (len > line * DUMP_BYTES) {
        for (int i = line * DUMP_BYTES; i < DUMP_BYTES + line * DUMP_BYTES; i++) {
            if (((int)sizeof(dump_buffer) - offset) < 0) {
                break;
            }

            if (i < len) {
                offset += snprintf(dump_buffer + offset,
                                sizeof(dump_buffer) - offset, "%02X ", data[i]);
            } else {
                offset += snprintf(dump_buffer + offset,
                                sizeof(dump_buffer) - offset, "%02X ", 0);
            }
        }
        offset += snprintf(dump_buffer + offset, sizeof(dump_buffer) - offset, "\n");
    }
    printf("date len: %d\n", len);
    printf("%s", dump_buffer);
}
