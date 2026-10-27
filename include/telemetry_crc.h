#ifndef TELEMETRY_CRC_H
#define TELEMETRY_CRC_H

#include <stdint.h>

uint16_t calculate_crc16(const uint8_t *data, uint16_t length);

#endif