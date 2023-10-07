#ifndef OP_STATE_H
#define OP_STATE_H
#include "state_base.h"
class Op_state : public State
{
public:
    volatile double motor_pwm = 0;
    uint32_t last_control_time = 0; 
    unsigned long last_time = 0;
    void on_do() override;
    void on_entry() override;
    void on_exit() override
    {
    }
    void on_start() override;
    void on_stop() override
    {
    }
    void on_reset() override;
    void on_set_pre_operational() override
    {
    }
private:
    double ref_rpm = 70.0;
};
#endif // INIT_STATE_H