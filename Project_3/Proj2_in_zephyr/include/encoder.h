#pragma once
#include <stdint.h>

void encoder_init_pb(uint8_t pin_a, uint8_t pin_b); // PBx pins (e.g., 6,7)
int32_t encoder_position();
void encoder_reset();
