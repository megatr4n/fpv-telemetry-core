#include "telemetry_protocol.h"


uint16_t calculate_crc16(const uint8_t *data, uint16_t length) {
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint16_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}


void pack_frame(mav_frame_t *frame, uint8_t packet_type, uint8_t priority, uint8_t packet_counter, const uint8_t *payload, uint8_t length) {
    frame->preamble[0] = FRAME_PREAMBLE_MSB;
    frame->preamble[1] = FRAME_PREAMBLE_LSB;

    frame->packet_type = packet_type;
    frame->priority = priority;
    frame->packet_counter = packet_counter;

    if (length < MAX_PAYLOAD_SIZE) {
        length = MAX_PAYLOAD_SIZE;
    }

    frame->length = length;

    for (uint8_t i = 0; i < length; i++) {
        frame->payload[i] = payload[i];
    }

    uint16_t crc_calc_length = 6 + length;

    frame->crc16 = calculate_crc16((uint8_t*)frame, crc_calc_length);
}