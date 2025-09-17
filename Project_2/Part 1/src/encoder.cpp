#include <avr/io.h>
#include <avr/interrupt.h>
#include "encoder.h"
#include "digital_in.h"
#include "digital_out.h"

static volatile int32_t s_pos = 0;
static uint8_t s_a_mask = 0;
static uint8_t s_b_mask = 0;
static uint8_t s_led_mask = 0;

void encoder_init(uint8_t a_bit, uint8_t b_bit, uint8_t led_bit)
{
    s_a_mask   = (uint8_t)(1u << a_bit);  
    s_b_mask   = (uint8_t)(1u << b_bit);  
    s_led_mask = (uint8_t)(1u << led_bit);

    Digital_in A(a_bit);  A.init();
    Digital_in B(b_bit);  B.init();
    Digital_out LED(led_bit);
    LED.init();

    PCICR  |= (1 << PCIE0);     
    PCMSK0 |= s_a_mask;          
    PCIFR  |= (1 << PCIF0);     

    sei(); 
}

int encoder_position(void)
{
    return (int)s_pos;
}

ISR(PCINT0_vect)
{
    uint8_t pinb = PINB;
    bool a = (pinb & s_a_mask) != 0;  
    bool b = (pinb & s_b_mask) != 0;  

    if (a == b) { s_pos--; } else { s_pos++; }
}
