#include "init_state.h"
#include "pre_op_state.h"
void Init_state::on_start()
{
  Serial.println("Starting Init State");
}

void Init_state::on_do()
{
  Serial.println("Currently initializing");
  delay(2000);
  Serial.println("Done initializing");
  this->context_->transition_to(new Pre_op_state);
}
void Init_state::on_entry()
{
  Serial.println("State is now INIT");
}
