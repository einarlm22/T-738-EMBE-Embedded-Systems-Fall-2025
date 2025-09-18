#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

void encoder_init(uint8_t a_bit, uint8_t b_bit, uint8_t led_bit = 5);
int encoder_position(void);

#endif
