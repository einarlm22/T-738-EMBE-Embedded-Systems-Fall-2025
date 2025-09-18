#include <Arduino.h>
#include "encoder.h"
#include "digital_out.h"

#define PRINT_MS 100     

#define CPR 1400            
#define RPM_WIN_MS 100

Digital_out led(5);

long last_pos = 0;
unsigned long last_rpm_ms = 0;
long rpm_val = 0;

int main() {
  init();
  Serial.begin(9600);

  encoder_init(0, 1, 5);

  unsigned long last = 0;
  while (1) {
    unsigned long now = millis();

    if (now - last_rpm_ms >= RPM_WIN_MS) {
      long pos = encoder_position();              
      long delta = pos - last_pos;                
      last_pos = pos;
      last_rpm_ms += RPM_WIN_MS;                  

      // RPM = delta * 600 / CPR  (delta is for 0.1 s)
      if (CPR > 0) rpm_val = (delta * 600L) / CPR;
      else         rpm_val = 0;
    }

    if (now - last >= PRINT_MS) {
      last = now;
      Serial.print(encoder_position());
      Serial.print(", rpm=");
      Serial.println(rpm_val);
    }
  }
}
