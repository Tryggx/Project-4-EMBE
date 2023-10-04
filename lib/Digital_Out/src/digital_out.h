// class digital_out
// used for init, set_hi, set_lo, toggle
// private var uint8_t pinMask
#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H
#include <avr/io.h>

class Digital_out
{
public:
    Digital_out(uint8_t pin);
    void init();
    void set_hi();
    void set_lo();
    void toggle();

private:
    uint8_t pinMask;
    volatile uint8_t *port;
};

#endif