#include <Arduino.h>
#include "encoder.h"
#include "motorpwm.h"
#include "p_controller.h"

#define CPR         1400
#define RPM_WIN_MS  4       // ~250 Hz
#define PRINT_MS    50
#define KP          1.0
#define REF_RPM     50
#define STEP_DELAY  200

long last_pos = 0;
unsigned long last_rpm_ms = 0;
long rpm_val = 0;

int main() {
  init();
  Serial.begin(115200);

  encoder_init(0, 1, 5);
  motor_pwm_init();

  motor_pwm_setA(0);
  motor_pwm_setB(0);

  P_controller pc(KP);

  unsigned long t0 = millis();
  unsigned long last_print = 0;

  while (1) {
    unsigned long now = millis();
    if (last_rpm_ms == 0) last_rpm_ms = now;
    unsigned long dt = now - last_rpm_ms;

    if (dt >= RPM_WIN_MS) {
      long pos = encoder_position();
      long delta = pos - last_pos;
      last_pos = pos;
      last_rpm_ms = now;

      long den = (long)CPR * (long)dt;
      rpm_val = (den > 0) ? (long)((delta * 60000L) / den) : 0;

      double ref = (now - t0 >= STEP_DELAY) ? (double)REF_RPM : 0.0;
      int pwm = pc.update(ref, (double)rpm_val);

      motor_pwm_setA((uint8_t)pwm);
      motor_pwm_setB(0);

      if (now - last_print >= PRINT_MS) {
        last_print = now;
        Serial.print(now - t0); Serial.print(", ");
        Serial.print((int)ref); Serial.print(", ");
        Serial.print((int)rpm_val); Serial.print(", ");
        Serial.println(pwm);
      }
    }
  }
}
