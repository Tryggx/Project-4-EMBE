#include "digital_in.h"

Digital_in::Digital_in(uint8_t set_pin)
{
    pinMask = (1 << set_pin % 8);
    if (set_pin / 8 == 0)
    {
        pin = &PIND;
        port = &PORTD;
        ddr = &DDRD;
    }
    else if (set_pin / 8 == 1)
    {
        pin = &PINB;
        port = &PORTB;
        ddr = &DDRB;
    }
    else if (set_pin / 8 == 2)
    {
        pin = &PINC;
        port = &PORTC;
        ddr = &DDRC;
    }
        
}

void Digital_in::init()
{
    *port |= pinMask;
    *ddr &= ~pinMask;
}

bool Digital_in::is_hi()
{
    return *pin & pinMask;
}

bool Digital_in::is_lo()
{
    return !is_hi();
}
