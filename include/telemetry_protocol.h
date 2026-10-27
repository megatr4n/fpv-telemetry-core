#ifndef TELEMETRY_PROTOCOL_H
#define TELEMETRY_PROTOCOL_H

#include <stdint.h>

#define FRAME_PREAMBLE_MSB 0x42
#define FRAME_PREAMBLE_LSB 0x53
#define MAX_PAYLOAD_SIZE   32
#define PRIORITY_CRITICAL  0
#define PRIORITY_NORMAL    1

#pragma pack(push, 1)

typedef struct {
    uint8_t preamble[2];
    uint8_t packet_type;
    uint8_t priority;
    uint8_t length;
    uint8_t packet_counter;
    uint8_t payload[MAX_PAYLOAD_SIZE];
    uint16_t crc16;
} mav_frame_t;

#pragma pack(pop)

uint16_t calculate_crc16(const uint8_t *data, uint16_t length);

void pack_frame(mav_frame_t *frame, uint8_t pkt_type, uint8_t priority, uint8_t packet_counter, const uint8_t *payload, uint8_t length);

#endif