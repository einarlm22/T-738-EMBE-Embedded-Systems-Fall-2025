#pragma once
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

class Digital_in {
public:
    Digital_in(const struct device* port, uint8_t pin, gpio_flags_t flags = GPIO_INPUT | GPIO_PULL_UP)
        : m_port(port), m_pin(pin) { gpio_pin_configure(m_port, m_pin, flags); }
    int read() const { return gpio_pin_get(m_port, m_pin); }
private:
    const struct device* m_port;
    uint8_t m_pin;
};
