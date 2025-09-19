#include <Arduino.h>
#include "encoder.h"
#include "digital_out.h"
#include "motorpwm.h"

#define PRINT_MS 20    
#define CPR 1400            
#define RPM_WIN_MS 20

long last_pos = 0;
unsigned long last_rpm_ms = 0;
long rpm_val = 0;

int main() {
  init();
  Serial.begin(9600);

  encoder_init(0, 1, 5);
  motor_pwm_init();


  motor_pwm_setA(255);  // D10 at 100%
  motor_pwm_setB(0);    // D11 at 0%

  unsigned long last = 0;

  while (1) {
    unsigned long now = millis();

    if (now - last_rpm_ms >= RPM_WIN_MS) {
      long pos = encoder_position();              
      long delta = pos - last_pos;                
      last_pos = pos;
      last_rpm_ms += RPM_WIN_MS;                  

      if (CPR > 0) rpm_val = (delta * 3000L) / CPR;
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