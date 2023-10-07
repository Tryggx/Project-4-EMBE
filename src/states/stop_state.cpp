#include "stop_state.h"
#include "init_state.h"
#include "op_state.h"

void Stop_state::on_do()
{
  this->context_->disable_main_serial_input = true;
  if (Serial.available() > 0)
  {
    // read the incoming byte:
    int command = Serial.read();
    if (command == 'c')
    {
      this->context_->transition_to(new Op_state);
      this->context_->disable_main_serial_input = false;
    }
  }
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
  Serial.println("FAULT: Please reset ('c') to start the motor");
}

void Stop_state::on_reset()
{
  Serial.println("Resetting");
  this->context_->transition_to(new Init_state);
}