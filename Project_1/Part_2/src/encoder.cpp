#include "encoder.h"

Encoder::Encoder(uint8_t a_bit, uint8_t b_bit, uint8_t led_bit)
: A(a_bit), B(b_bit), led(led_bit), lastA(false), pos(0) {}

void Encoder::init() {
    A.init();        
    B.init();            
    lastA = A.is_hi();
}

void Encoder::sample_once() {
    bool a = A.is_hi();
    if (a != lastA) {
        bool b = B.is_hi();
        if (a == b) { pos--; } else { pos++; } 
        lastA = a;
    }
    
}

int32_t Encoder::position() const {
    return pos;
}
