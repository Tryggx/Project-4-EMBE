#ifndef STOP_STATE_H
#define STOP_STATE_H
#include "state_base.h"
class Stop_state : public State
{
public:
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
};
#endif // INIT_STATE_H