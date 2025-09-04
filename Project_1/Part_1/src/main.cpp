#include <util/delay.h>
#include "encoder.h"

#define SAMPLE_US 100

Digital_out led(5);

int main() {
  Encoder enc(0, 1, 5);
  enc.init();
  led.init();

  while (1) {
    enc.sample_once();
    _delay_us(SAMPLE_US);
    led.set_hi();
    led.set_lo(); 
  }
}