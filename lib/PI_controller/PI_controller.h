#pragma once
class PI_controller
{
public:
    double update(double ref, double actual);
    PI_controller(double Kp);

private:
    volatile double Kp;

};