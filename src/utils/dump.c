#include "stdio.h"
#include "stdint.h"

static inline char to_printable(uint8_t c)
{
    if ((c >= 0x20) && (c <= 0x7E)) {
        return (char)c;
    } else {
        return '.';
    }
}

static void dump_line(const uint8_t *data, int len)
{
    char buffer[16 * 4 + 8];
    int total = 0;

    if (len > 16) {
        len = 16;
    }

    for (int i = 0; i < len; i++) {
        total += snprintf(buffer + total, sizeof(buffer) - (size_t)total, "%02X ", data[i]);
    }
    for (int i = len; i < 16; i++) {
        total += snprintf(buffer + total, sizeof(buffer) - (size_t)total, "%02X ", data[i]);
    }
    total += snprintf(buffer + total, sizeof(buffer) - (size_t)total, "\t");
    for (int i = 0; i < len; i++) {
        total +=
            snprintf(buffer + total, sizeof(buffer) - (size_t)total, "%c", to_printable(data[i]));
    }
    // DBGLOG_LIB_RAW("%s\n", buffer);
}

/**
 * @brief dump a piece of data
 *
 * @param data the data to dump
 * @param len length of the data
 */
void dump_bytes(const uint8_t *data, int len)
{
    int line_count;

    line_count = len / 16;
    for (int i = 0; i < line_count; i++) {
        dump_line(data + i * 16, 16);
    }

    if (len > line_count * 16) {
        dump_line(data + line_count * 16, len - line_count * 16);
    }
}
