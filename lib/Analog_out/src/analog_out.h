#ifndef ANALOG_OUT_H
#define ANALOG_OUT_H

#include <timer_msec.h>
#include <digital_out.h>
#include <avr/interrupt.h>

class Analog_out
{
public:
    Analog_out(int pin);
    void init(int frequency = 1000);
    void set(float duty_cycle);

private:
    Timer_msec timer;

public:    
    Digital_out dout;
};


#endif // ANALOG_OUT_H