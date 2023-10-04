#ifndef DIGITAL_IN_H
#define DIGITAL_IN_H
#include <avr/io.h>
class Digital_in
{
public:
    Digital_in(uint8_t pin);
    void init();
    bool is_hi();
    bool is_lo();

private:
    uint8_t pinMask;
    volatile uint8_t *port;
};

#endif