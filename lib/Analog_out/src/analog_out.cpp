#include <avr/io.h>
#include "analog_out.h"
#if defined(PART_3_A21) || !defined(PART_1_A21) && !defined(PART_2_A21) && !defined(PART_3_A21)
Analog_out::Analog_out(int pin) : dout(pin)
{
}

void Analog_out::init(int frequency)
{
    dout.init();
    timer.init(frequency, 0.0);
    sei();
}

void Analog_out::set(float duty_cycle)
{
    timer.set_duty_cycle(duty_cycle);
}
#endif
