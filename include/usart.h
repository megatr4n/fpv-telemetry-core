#ifndef USART_H

#include "stm32f1xx_hal.h"
#include "ring_buffer.h"

extern ring_buffer_t rx_rb;

void usart1_init(void);
void usart1_transmit(const uint8_t *data, uint16_t len);



#endif