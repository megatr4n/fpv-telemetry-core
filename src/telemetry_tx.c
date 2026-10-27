#include "telemetry_tx.h"
#include "telemetry_crc.h"

void pack_frame(mav_frame_t *frame, uint8_t pkt_type, uint8_t priority, uint8_t packet_counter, const uint8_t *payload, uint8_t length) {
    frame->preamble[0] = FRAME_PREAMBLE_MSB;
    frame->preamble[1] = FRAME_PREAMBLE_LSB;

    frame->packet_type = pkt_type;
    frame->priority = priority;
    frame->packet_counter = packet_counter;

    if (length > MAX_PAYLOAD_SIZE) {
        length = MAX_PAYLOAD_SIZE;
    }
    frame->length = length;

    for (uint8_t i = 0; i < length; i++) {
        frame->payload[i] = payload[i];
    }

    uint16_t crc_calc_length = 6 + length;
    frame->crc16 = calculate_crc16((const uint8_t *)frame, crc_calc_length);
}