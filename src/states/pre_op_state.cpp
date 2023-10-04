#include "pre_op_state.h"
#include "init_state.h"

void Pre_op_state::on_do()
{

  if (millis() - this->last_time > 500)
  {
    this->last_time = millis();
    this->context_->io->led.toggle();
  }
}

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