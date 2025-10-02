#pragma once
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

class Digital_out {
public:
    Digital_out(const struct device* port, uint8_t pin)
        : m_port(port), m_pin(pin) { gpio_pin_configure(m_port, m_pin, GPIO_OUTPUT); }
    void set(int v) { gpio_pin_set(m_port, m_pin, v ? 1 : 0); }
    void toggle() { gpio_pin_toggle(m_port, m_pin); }
private:
    const struct device* m_port;
    uint8_t m_pin;
};
