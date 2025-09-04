#include <util/delay.h>
#include <Arduino.h>     
#include "encoder.h"

#define SAMPLE_US 100     
#define PRINT_MS  10   

Digital_out led(5);

int main() {
  init();                 
  Serial.begin(9600);    

  Encoder enc(0, 1, 5);
  enc.init();

  unsigned long last_print = 0;

  while (1) {
    enc.sample_once();           
    _delay_us(SAMPLE_US);
    unsigned long now = millis();
    if (now - last_print >= PRINT_MS) {
      last_print = now;
      Serial.println(enc.position());  
    }
    led.set_hi();
    led.set_lo(); 
  }
}