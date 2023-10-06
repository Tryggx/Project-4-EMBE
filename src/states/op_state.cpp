#include "op_state.h"
#include "init_state.h"
#include "stop_state.h"
void Op_state::on_do()
{
  // Serial.println(this->motor_pwm);
  if (micros() - this->last_control_time > 4000)
  {
    this->last_control_time = micros();
    double actual = this->context_->io->encoder.getRpm();
    double output = this->context_->io->controller.update(40, actual);
    this->motor_pwm = output / 105;
    this->context_->io->motor.set(this->motor_pwm);
  }
  if (this->context_->io->fault_pin.is_hi())
  {
    this->context_->transition_to(new Stop_state);
  }
}

void Op_state::on_start()
{
}

void Op_state::on_entry()
{
  Serial.println("Starting Motor");
  Serial.print("Using Kp: ");
  Serial.println(this->context_->io->controller.get_Kp());
  Serial.print("Using Ti: ");
  Serial.println(this->context_->io->controller.get_Ti());
}

void Op_state::on_reset()
{

  Serial.println("Resetting");
  this->context_->transition_to(new Init_state);
}