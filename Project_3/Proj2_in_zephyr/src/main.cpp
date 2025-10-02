#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include "encoder.h"
#include "motorpwm.h"
#include "p_controller.h"

/* params */
#define CPR         1400
#define SAMPLE_MS   4
#define PRINT_MS    50
#define REF_RPM     60
#define KP_GAIN     6.0
#define PWM_HZ      20000

int main(void)
{
    encoder_init_pb(6, 7);     // PB6, PB7
    motor_pwm_init(PWM_HZ);

    P_controller pc(KP_GAIN);

    int64_t t0 = k_uptime_get();
    int64_t last_ctrl = t0, last_print = t0;
    int32_t last_cnt = 0;
    double rpm = 0.0;

    while (1) {
        int64_t now = k_uptime_get();

        if (now - last_ctrl >= SAMPLE_MS) {
            uint32_t dt_ms = (uint32_t)(now - last_ctrl);
            last_ctrl = now;

            int32_t pos = encoder_position();
            int32_t delta = pos - last_cnt;
            last_cnt = pos;

            rpm = (dt_ms > 0 && CPR > 0)
                ? ((double)delta * 60000.0) / (CPR * (double)dt_ms)
                : 0.0;

            double ref = (now - t0 >= 200) ? (double)REF_RPM : 0.0;
            int pwm = pc.update(ref, rpm);

            // DRV8833 forward: AIN1=PWM (PA8), AIN2=0% (PA11)
            motor_pwm_setA((uint8_t)pwm);
            motor_pwm_setB(0);

            if (now - last_print >= PRINT_MS) {
                last_print = now;
                printk("%lld, %d, %d, %d\n",
                       (long long)(now - t0),
                       (int)ref, (int)rpm, pwm);
            }
        }
        k_sleep(K_MSEC(1));
    }
}
