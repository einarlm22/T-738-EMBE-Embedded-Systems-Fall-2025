#include <Arduino.h>
#include "encoder.h"
#include "digital_out.h"

#define PRINT_MS 10     

Digital_out led(5);

int main() {
  init();
  Serial.begin(9600);

  encoder_init(0, 1, 5);

  unsigned long last = 0;
  while (1) {
    unsigned long now = millis();
    if (now - last >= PRINT_MS) {
      last = now;
      Serial.println(encoder_position()); 
    }
    led.set_hi();
    led.set_lo(); 
  }
}
