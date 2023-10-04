#include <avr/io.h>
#include "timer_msec.h"

Timer_msec::Timer_msec()
{
}
// if part 1, use this
#ifdef PART_1_A21
void Timer_msec::init(int frequency)
{
    // Modified code from lecture 2.2
    //  this code sets up timer1 for a 1s @ 16Mhz Clock (mode 4)
    //  counting 16000000/1024 cycles of a clock prescaled by 1024
    TCCR1A = 0; // set timer1 to normal operation (all bits in control registers A and B set to zero)
    TCCR1B = 0; //
    TCNT1 = 0;  // initialize counter value to 0
    // 16Mhx - 1 second, divide by 1024 to get 15625 ticks per second
    // then divide by 1000 to get 15.625 ticks per millisecond
    OCR1A = (16000000 / 1024) / 1000.0 * frequency - 1;
    TCCR1B |= (1 << WGM12);              // clear the timer on compare match A
    TIMSK1 |= (1 << OCIE1A);             // set interrupt on compare match A
    TCCR1B |= (1 << CS12) | (1 << CS10); // set prescaler to 1024 and start the timer
}

#endif

// default behavior if no parts are defined
#if defined(PART_2_A21) || defined(PART_3_A21) || !defined(PART_1_A21) && !defined(PART_2_A21) && !defined(PART_3_A21)
void Timer_msec::init(int frequency, float duty_cycle)
{
    // Slighly modified code from lecture 2.2
    TCCR1A = 0; // set timer1 to normal operation (all bits in control registers A and B set to zero)
    TCCR1B = 0;
    TCNT1 = 0; // initialize counter value to 0

    OCR1A = ((16000000 / 256) / frequency) - 1;

    OCR1B = OCR1A * duty_cycle;          // assign target count to compare register B (should not exceed value of OCR1A)
    //if duty cycle is 1.0, turn off timer b
    TCCR1B |= (1 << WGM12);              // clear the timer on compare match A
    TIMSK1 |= (1 << OCIE1A);             // set interrupt on compare match A
    TIMSK1 |= (1 << OCIE1B);             // set interrupt on compare match B
    TCCR1B |= (1 << CS12); // set prescaler to 1024 and start the timer
}
#endif

// default behavior if no parts are defined
#if defined(PART_3_A21) || !defined(PART_1_A21) && !defined(PART_2_A21) && !defined(PART_3_A21)
void Timer_msec::set_duty_cycle(float duty_cycle)
{
    OCR1B = OCR1A * duty_cycle; // assign target count to compare register B (should not exceed value of OCR1A)
}
#endif
