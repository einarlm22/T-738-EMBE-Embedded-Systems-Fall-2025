#include "motorpwm.h"
#include <avr/io.h>

void motor_pwm_init()
{
    // D10 (PB2/OC1B)
    DDRB |= (1 << PB2);
    // 8-bit Fast PWM on Timer1
    TCCR1A = (1 << WGM10) | (1 << COM1B1);
    TCCR1B = (1 << WGM12) | (1 << CS10); // prescaler 1 (~31 kHz)
    OCR1B = 0;

    // D11 (PB3/OC2A)
    DDRB |= (1 << PB3);
    // Fast PWM on Timer2
    TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2A1);
    TCCR2B = (1 << CS20); // prescaler 1 (~62 kHz)
    OCR2A = 0;
}

void motor_pwm_setA(uint8_t duty)
{
    OCR1B = duty; // D10
}

void motor_pwm_setB(uint8_t duty)
{
    OCR2A = duty; // D11
}