#include "motorpwm.h"

extern "C" {
#include <stm32l4xx_ll_bus.h>
#include <stm32l4xx_ll_gpio.h>
#include <stm32l4xx_ll_tim.h>
#include <stm32l4xx_ll_system.h>
#include <stm32l4xx.h>
}

static uint32_t g_arr = 0; // Auto-reload value (period ticks)

static void gpio_pa8_pa11_to_af1()
{
    // Enable GPIOA clock
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    // PA8 -> AF1 (TIM1_CH1)
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_8, LL_GPIO_AF_1);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_8, LL_GPIO_PULL_NO);

    // PA11 -> AF1 (TIM1_CH4)
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_11, LL_GPIO_AF_1);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_11, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_11, LL_GPIO_PULL_NO);
}

static void tim1_setup(uint32_t pwm_hz)
{
    if (pwm_hz == 0) pwm_hz = 20000; // 20 kHz default

    // TIM1 clock is on APB2. In Zephyr’s default Nucleo L432KC config APB2 = 80 MHz (no x2 factor).
    uint32_t tim_clk = SystemCoreClock;

    // Choose prescaler so that ARR fits in 16 bits
    uint32_t psc = 0;
    uint32_t ticks = tim_clk / pwm_hz;
    while (ticks > 0x10000) { // need ARR <= 0xFFFF
        ++psc;
        ticks = (tim_clk / (psc + 1U)) / pwm_hz;
    }
    if (ticks == 0) ticks = 1;
    g_arr = ticks - 1U;

    // Enable TIM1 clock
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

    // Basic timer config
    LL_TIM_DisableCounter(TIM1);
    LL_TIM_SetPrescaler(TIM1, psc);
    LL_TIM_SetAutoReload(TIM1, g_arr);
    LL_TIM_SetCounterMode(TIM1, LL_TIM_COUNTERMODE_UP);
    LL_TIM_EnableARRPreload(TIM1);

    // PWM mode on CH1 (PA8)
    LL_TIM_OC_SetMode(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
    LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH1);
    LL_TIM_OC_SetPolarity(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH);
    LL_TIM_OC_SetCompareCH1(TIM1, 0);
    LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);

    // PWM mode on CH4 (PA11)
    LL_TIM_OC_SetMode(TIM1, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM1);
    LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH4);
    LL_TIM_OC_SetPolarity(TIM1, LL_TIM_CHANNEL_CH4, LL_TIM_OCPOLARITY_HIGH);
    LL_TIM_OC_SetCompareCH4(TIM1, 0);
    LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH4);

    // Enable main output (advanced-control timer)
    LL_TIM_EnableAllOutputs(TIM1);

    // Start
    LL_TIM_GenerateEvent_UPDATE(TIM1);
    LL_TIM_EnableCounter(TIM1);
}

void motor_pwm_init(uint32_t pwm_hz)
{
    gpio_pa8_pa11_to_af1();
    tim1_setup(pwm_hz);
}

static inline uint32_t duty_to_ccr(uint8_t duty)
{
    // CCR = duty/255 * (ARR+1)
    return (uint32_t)((uint64_t)duty * (g_arr + 1ULL) / 255ULL);
}

void motor_pwm_setA(uint8_t duty)
{
    if (!g_arr) return;
    LL_TIM_OC_SetCompareCH1(TIM1, duty_to_ccr(duty));
}

void motor_pwm_setB(uint8_t duty)
{
    if (!g_arr) return;
    LL_TIM_OC_SetCompareCH4(TIM1, duty_to_ccr(duty));
}
