#include <avr/io.h>
#include "timer_usec.h"

Timer_usec::Timer_usec()
{
}


void Timer_usec::init()
{
    TCCR0A = 0; // set timer1 to normal operation (all bits in control registers A and B set to zero)
    TCCR0B = 0;
    TCNT0 = 0; // initialize counter value to 0

    OCR0A = 9; //timer counts every 5 microseconds

    TCCR0A |= (1 << WGM01);              // clear the timer on compare match A
    TIMSK0 |= (1 << OCIE0A);             // set interrupt on compare match A
    TCCR0B |= (1 << CS01);
    sei();
     // set prescaler to 8
}

