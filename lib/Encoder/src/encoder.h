#ifndef ENCODER_H
#define ENCODER_H
#include <avr/io.h>
#include "digital_in.h"
#include "timer_usec.h"
class Encoder
{
public:
    Encoder(uint8_t pin1, uint8_t pin2, float gear_ratio, float pulses_per_rev);
    void init();
    int position();
    bool pollEncoder();
    void onChange();
    float getRpm();
    

public:
    float ang_vel;
    Timer_usec timer;

private:
    float rpm;
    int encoderPin1;
    int encoderPin2;
    uint16_t currentPos;
    Digital_in encoder1;
    Digital_in encoder2;
    bool lastval;
    uint32_t lastPulse;
    int gearRatio;
    int pulsesPerRev;
    int rpmCounter = 0;
};

#endif