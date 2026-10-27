#ifndef TELEMETRY_TX_H
#define TELEMETRY_TX_H

#include "telemetry_types.h"

void pack_frame(mav_frame_t *frame, uint8_t pkt_type, uint8_t priority, uint8_t packet_counter, const uint8_t *payload, uint8_t length);

#endif