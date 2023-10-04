#include "pre_op_state.h"
#include "init_state.h"

void Pre_op_state::on_start()
{
  Serial.println("Starting");
  Serial.println("Next state not implemented");
}

void Pre_op_state::on_entry()
{
  Serial.println("Now in pre-op state");
  Serial.println("State is now PRE_OP");
}

void Pre_op_state::on_reset()
{
  Serial.println("Resetting");
  this->context_->transition_to(new Init_state);
}