#include "P_controller.h"
#include <Arduino.h>
P_controller::P_controller(double kp)
{
    Kp = kp;
}

double P_controller::update(double ref, double actual)
{
    volatile double error = ref - actual;
    volatile double output = Kp * error;
    return output;
}

