#ifndef TELEMETRY_TYPES_H
#define TELEMETRY_TYPES_H

#include <stdint.h>

#define FRAME_PREAMBLE_MSB 0x42
#define FRAME_PREAMBLE_LSB 0x53
#define MAX_PAYLOAD_SIZE   32

#define PRIORITY_LOW    0x00
#define PRIORITY_MEDIUM 0x01
#define PRIORITY_HIGH   0x02

#pragma pack(push, 1)
typedef struct {
    uint8_t  preamble[2];
    uint8_t  packet_type;
    uint8_t  priority;
    uint8_t  packet_counter;
    uint8_t  length;
    uint8_t  payload[MAX_PAYLOAD_SIZE];
    uint16_t crc16;
} mav_frame_t;
#pragma pack(pop)

typedef enum {
    RX_WAIT_PREAMBLE_MSB,
    RX_WAIT_PREAMBLE_LSB,
    RX_WAIT_HEADER,
    RX_WAIT_PAYLOAD,
    RX_WAIT_CRC
} rx_state_t;

typedef struct {
    rx_state_t  state;
    mav_frame_t frame;
    uint8_t     byte_index;
} parser_context_t;

#endif