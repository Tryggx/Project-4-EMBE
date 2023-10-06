#include "stop_state.h"
#include "init_state.h"

void Stop_state::on_do()
{
  if (millis() - this->last_time > 250)
  {
    this->last_time = millis();
    this->context_->io->led.toggle();
  }
}

void Stop_state::on_start()
{
}

void Stop_state::on_entry()
{
  this->context_->io->motor.set(0);
  Serial.println("FAULT: Please reset ('r') to reset the controller");
}

void Stop_state::on_reset()
{
  Serial.println("Resetting");
  this->context_->transition_to(new Init_state);
}