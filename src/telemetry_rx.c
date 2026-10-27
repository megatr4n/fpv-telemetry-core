#include "telemetry_rx.h"
#include "telemetry_crc.h"

#include <stdbool.h>

void parser_init(parser_context_t *ctx) {
    ctx->state =  FRAME_PREAMBLE_MSB;
    ctx->byte_index = 0;
}

bool parse_byte(parser_context_t *ctx, uint8_t byte) {
    switch (ctx->state) {

        case RX_WAIT_PREAMBLE_MSB:
        if (byte == FRAME_PREAMBLE_MSB) {
            ctx->frame.preamble[0] = byte;
            ctx->state = RX_WAIT_PREAMBLE_LSB;
        }
        break;

        case FRAME_PREAMBLE_LSB:
        if (byte == FRAME_PREAMBLE_LSB) {
            ctx->frame.preamble[1] = byte;
            ctx->state = RX_WAIT_HEADER;
            ctx->byte_index = 0;
        } else {
            ctx->state = FRAME_PREAMBLE_LSB;
        }

        case RX_WAIT_HEADER:
            if (ctx->byte_index == 0)
                ctx->frame.packet_type = byte;
            else if 
                (ctx->byte_index == 1)
                 ctx->frame.priority = byte;
            else if
                (ctx->byte_index == 2)
                 ctx->frame.packet_counter = byte;
            else if
                (ctx->byte_index == 3) {
            if (byte > MAX_PAYLOAD_SIZE) {
                ctx->state = RX_WAIT_PREAMBLE_MSB;
                break;
                }
                ctx->frame.length = byte;
                ctx->byte_index = 0;

                if (ctx->frame.length == 0) {
                    ctx->state = RX_WAIT_CRC;
                } else {
                    ctx->state = RX_WAIT_PAYLOAD;
                }
                break;
            }
            ctx->byte_index++;
            break;

            case RX_WAIT_PAYLOAD:
                ctx->frame.payload[ctx->byte_index] = byte;
                ctx->byte_index++; 
                if (ctx->byte_index > ctx->frame.length) {
                    ctx->state = RX_WAIT_CRC;
                    ctx->byte_index = 0;
                } break;

                case RX_WAIT_CRC:
                ((uint8_t *)&ctx->frame.crc16)[ctx->byte_index] = byte;
                ctx->byte_index++;
                if (ctx->byte_index >= 2) {
                    ctx->state = RX_WAIT_PREAMBLE_MSB;

                    uint16_t expected_crc = calculate_crc16((const uint8_t *)&ctx->frame.crc16, 6 + ctx->frame.length);
                    if (expected_crc == ctx->frame.crc16) {
                        return true;
                    }
                }
                break;
            }
            return false;
        }
