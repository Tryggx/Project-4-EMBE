#include "op_state.h"
#include "init_state.h"
#include "stop_state.h"
void Op_state::on_do()
{
  
  if (micros() - this->last_control_time > 4000)
  {
    this->last_control_time = micros();
    double actual = this->context_->io->encoder.getRpm();
    double output = this->context_->io->controller.update(ref_rpm, actual);
    this->motor_pwm = output / 105;
    this->context_->io->motor.set(this->motor_pwm);
    //print csv, Actual rpm, requested rpm and pwm and millis
    // Serial.print(actual);
    // Serial.print(",");
    // Serial.print(ref_rpm);
    // Serial.print(",");
    // Serial.print(this->motor_pwm);
    // Serial.print(",");
    // Serial.println(millis());

  }
  if (this->context_->io->fault_pin.is_lo())
  {
    this->context_->transition_to(new Stop_state);
  }
  // if (millis() % 3000 < 30){
  //   ref_rpm == 50.0 ? ref_rpm = 90.0 : ref_rpm = 50.0;
  // }
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