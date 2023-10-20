#pragma once

class P_controller
{
public:
    virtual double update(double ref, double actual);
    P_controller(double Kp);

private:
    volatile double Kp;

};


class PI_controller : public P_controller
{
public:
    PI_controller(double Kp, double integrationTime, double timeStep);
    double update(double ref, double actual) override;
    void reset_integral();
    void set_Kp(double Kp);
    void set_Ti(double Ti);
    double get_Ti();
    double get_Kp();
    double get_integral();
    bool is_saturated = false;
    

private:
    double integration_time;
    double Kp = 0.0;
    double time_step;
    double previous_error = 0.0;
    double integral = 0.0;

    const double PWM_MAX = 105.0;
    const double PWM_MIN = -105.0;
};