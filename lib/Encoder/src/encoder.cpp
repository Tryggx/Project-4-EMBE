#include "encoder.h"
#include <util/delay.h>
#include <Arduino.h>
#include <avr/interrupt.h>

Encoder::Encoder(uint8_t pin1, uint8_t pin2, float gear_ratio, float pulses_per_rev) : encoder1(pin1), encoder2(pin2)
{
    encoderPin1 = pin1;
    encoderPin2 = pin2;
    gearRatio = gear_ratio;
    pulsesPerRev = pulses_per_rev;
}

int Encoder::position()
{
    return currentPos;
}

void Encoder::init()
{
    encoder1.init();
    encoder2.init();
    // enable port change interrupt on port  pin / 8
    PCICR |= (1 << PCIE0);
    /// use pin 9
    PCMSK0 |= (1 << encoderPin1 % 8);
    sei();
    currentPos = 0;
    timer.init();
}
void Encoder::onChange()
{
    bool val = encoder1.is_hi();
    float tempRpm = rpm;
    if (val) {
        rpmCounter++;
        if (rpmCounter >= 10) {
        tempRpm =  600000000.0 / ((timer.micros - lastPulse) * pulsesPerRev * gearRatio);
        lastPulse = timer.micros;
        rpmCounter = 0;
        }
    }
    
    // 7 pulses per revolution, gear ratio 100, 60 seconds per minute
    //float tempRpm = 1*1000000 / ((micros() - lastPulse) * pulsesPerRev * gearRatio * 60);
    if (encoder2.is_hi() != val)
    {
        currentPos++;
        rpm = tempRpm;
    }
    else
    {
        currentPos--;
        rpm = -tempRpm;
    }
}
bool Encoder::pollEncoder()
{
    bool val = encoder1.is_hi();
    bool enc2 = encoder2.is_hi();
    bool retval = val != lastval;
    if (val != lastval)
    {
        if (enc2 != val)
        {
            currentPos++;
        }
        else
        {
            currentPos--;
        }
        lastval = val;
    }
    return retval;
}

float Encoder::getRpm()
{
    if (timer.micros - lastPulse > 1000000)
    {
        rpm = 0;
    }
    return rpm;
}