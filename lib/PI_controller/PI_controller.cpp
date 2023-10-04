    #include "PI_controller.h"
    #include <Arduino.h>
    PI_controller::PI_controller(double kp)
    {
        Kp = kp;
    }
    
    double PI_controller::update(double ref, double actual)
    {
        volatile double error = ref - actual;
        volatile double output = Kp * error;
        return output;
    }