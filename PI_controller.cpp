#include "PI_controller.h"

PI_controller::PI_controller(double Kp, double integrationTime, double timeStep)
    : integration_time(integrationTime), time_step(timeStep)
{
    this->Kp = Kp;
    reset_integral();
}

double PI_controller::update(double ref, double actual)
{
    double error = ref - actual;
    double output = 0.0;
    // integral = 0.0;
    output = Kp * (error + (1 / integration_time) * integral);
    if (output <= -120)
    {
        output = -120;
        is_saturated = true;
    }
    else if (output >= 120)
    {
        output = 120;
        is_saturated = true;
    }
    else
    {
        is_saturated = false;
    }

    if(!is_saturated) {
    integral += error * time_step;
    }

    return output;
}
void PI_controller::reset_integral()
{
    integral = 0.0;
}

void PI_controller::set_Kp(double Kp)
{
    this->Kp = Kp;
}

void PI_controller::set_Ti(double Ti)
{
    this->integration_time = Ti;
}

double PI_controller::get_Ti()
{
    return this->integration_time;
}

double PI_controller::get_Kp()
{
    return this->Kp;
}

double PI_controller::get_integral()
{
    return this->integral;
}