#ifndef PRE_OP_STATE_H
#define PRE_OP_STATE_H
#include "state_base.h"
class Pre_op_state : public State
{
public:
    void on_do() override
    {
    }
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