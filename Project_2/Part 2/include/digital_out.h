#include <stdint.h>
#include <Arduino.h>
class Digital_out
{
public:
    Digital_out(int port_no)
    {
        pinMask = (1<<port_no);
    }
    void init()
    {
        DDRB |= (pinMask);
    } 
    void set_hi()
    {
        PORTB |= (pinMask);
    }
    void set_lo()
    {
        PORTB &= ~(pinMask);
    }
    void toggle()
    {
        PORTB ^= (pinMask);
    }
private:
    int8_t pinMask;
};


