#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include "digital_in.h"   
#include "digital_out.h" 

class Encoder {
public:
    Encoder(uint8_t a_bit, uint8_t b_bit, uint8_t led_bit = 5);

    void init();          
    void sample_once();   
    int32_t position() const;

private:
    Digital_in  A, B;
    Digital_out led;
    bool        lastA;
    volatile int32_t pos;
};

#endif
