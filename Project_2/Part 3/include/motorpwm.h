#ifndef MOTORPWM_H
#define MOTORPWM_H

#include <stdint.h>
#include <Arduino.h>

// Initialize PWM on D10 (Timer1 OC1B) and D11 (Timer2 OC2A)
void motor_pwm_init();

// Set PWM duty on D10 (0–255)
void motor_pwm_setA(uint8_t duty);

// Set PWM duty on D11 (0–255)
void motor_pwm_setB(uint8_t duty);

#endif