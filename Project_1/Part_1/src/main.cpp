#include <util/delay.h>
#include "encoder.h"

#define SAMPLE_US 50

int main() {
  Encoder enc(0, 1, 5);
  enc.init();

  while (1) {
    enc.sample_once();
    _delay_us(SAMPLE_US);
  }
}