#include "digital_in.h"

Digital_in::Digital_in(uint8_t pin)
{
    pinMask = (1 << pin % 8);
    if (pin / 8 == 0)
    {
        port = &PIND;
    }
    else if (pin / 8 == 1)
    {
        port = &PINB;
    }
    else if (pin / 8 == 2)
    {
        port = &PINC;
    }
        
}

void Digital_in::init()
{
    *port &= ~pinMask; // set as input
}

bool Digital_in::is_hi()
{
    return *port & pinMask;
}

bool Digital_in::is_lo()
{
    return !is_hi();
}
