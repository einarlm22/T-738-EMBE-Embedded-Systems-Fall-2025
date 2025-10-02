#include "encoder.h"
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

static volatile int32_t g_count = 0;
static volatile uint8_t g_last_ab = 0;
static struct gpio_callback g_cb;

static void enc_edge_cb(const struct device* port, struct gpio_callback* cb, uint32_t pins)
{
    int a = gpio_pin_get(port, 6); // PB6
    int b = gpio_pin_get(port, 7); // PB7
    uint8_t ab = ((a & 1) << 1) | (b & 1);

    int8_t step = 0;
    switch ((g_last_ab << 2) | ab) {
        case 0b0001: case 0b0111: case 0b1110: case 0b1000: step = +1; break;
        case 0b0010: case 0b0100: case 0b1101: case 0b1011: step = -1; break;
        default: step = 0; break;
    }
    g_count += step;
    g_last_ab = ab;
}

void encoder_init_pb(uint8_t pin_a, uint8_t pin_b)
{
    const struct device* gpiob = DEVICE_DT_GET(DT_NODELABEL(gpiob));
    gpio_pin_configure(gpiob, pin_a, GPIO_INPUT | GPIO_PULL_UP);
    gpio_pin_configure(gpiob, pin_b, GPIO_INPUT | GPIO_PULL_UP);

    int a0 = gpio_pin_get(gpiob, pin_a), b0 = gpio_pin_get(gpiob, pin_b);
    g_last_ab = ((a0 & 1) << 1) | (b0 & 1);

    gpio_pin_interrupt_configure(gpiob, pin_a, GPIO_INT_EDGE_BOTH);
    gpio_pin_interrupt_configure(gpiob, pin_b, GPIO_INT_EDGE_BOTH);

    gpio_init_callback(&g_cb, enc_edge_cb, BIT(pin_a) | BIT(pin_b));
    gpio_add_callback(gpiob, &g_cb);
}

int32_t encoder_position() { return g_count; }
void encoder_reset() { g_count = 0; }
