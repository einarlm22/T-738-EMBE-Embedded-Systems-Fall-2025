#pragma once
#include <stdint.h>

void motor_pwm_init(uint32_t pwm_hz = 20000); // default 20 kHz
void motor_pwm_setA(uint8_t duty);            // TIM1_CH1 -> PA8
void motor_pwm_setB(uint8_t duty);            // TIM1_CH4 -> PA11
