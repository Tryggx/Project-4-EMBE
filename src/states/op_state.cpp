#include "op_state.h"
#include "init_state.h"
void Op_state::on_do()
{
  // Serial.println(this->motor_pwm);
  if (this->context_->io->encoder.timer.micros - this->last_control_time > 4000)
  {
    this->last_control_time = this->context_->io->encoder.timer.micros;
    double actual = this->context_->io->encoder.getRpm();
    double output = this->context_->io->controller.update(80, actual);
    output > 1 ? output = 1 : output < 0 ? output = 0
                                         : output = output;
    this->motor_pwm = output;
    this->context_->io->motor.set(this->motor_pwm);
  }
  if (this->context_->io->fault_pin.is_hi())
  {
    this->context_->transition_to(new Init_state);
  }
}

void Op_state::on_start()
{
}

void Op_state::on_entry()
{
  Serial.println("Starting Motor");
  Serial.println(this->context_->io->fault_pin.is_hi());
}

void Op_state::on_reset()
{

  Serial.println("Resetting");
  this->context_->transition_to(new Init_state);
}