#ifndef TELEMETRY_RX_H
#define TELEMETRY_RX_H

#include <stdbool.h>
#include "telemetry_types.h"

void parser_init(parser_context_t *ctx);
bool parse_byte(parser_context_t *ctx, uint8_t byte);

#endif