// class digital_out.cpp
// used for init, set_hi, set_lo, toggle
// private var uint8_t pinMask

#include <avr/io.h>
#include "digital_out.h"

Digital_out::Digital_out(uint8_t pin)
{
    pinMask = (1 << (pin % 8));
    if (pin / 8 == 0)
    {
        port = &PORTD;
    }
    else if (pin / 8 == 1)
    {
        port = &PORTB;
    }
}

void Digital_out::init()
{
    *port |= pinMask; // set as output
}

void Digital_out::set_hi()
{
    *port |= pinMask;
}

void Digital_out::set_lo()
{
    *port &= ~pinMask;
}

void Digital_out::toggle()
{
    *port ^= pinMask;
}