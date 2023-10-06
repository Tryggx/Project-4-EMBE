#include "init_state.h"
#include "pre_op_state.h"
#include "op_state.h"
void Init_state::on_start()
{
  Serial.println("Starting Init State");
}

void Init_state::on_do()
{
  Serial.println("Currently initializing");
  this->context_->io->motor.set(0);
  /*       void init_hardware(uint8_t led_pin,
                     uint8_t analog_out_pin,
                     uint8_t encoder_pinA,
                     uint8_t encoder_pinB,
                     uint8_t fault_pin_pin,
                     float gear_ratio,
                     float ppr,
                     double kp)*/
  //this->context_->init_hardware(
  this->context_->io->led.init();
  this->context_->io->motor.init(2000);
  this->context_->io->encoder.init();
  this->context_->io->fault_pin.init();
  Serial.println("Done initializing");
  this->context_->transition_to(new Pre_op_state);
}
void Init_state::on_entry()
{
  Serial.println("State is now INIT");
}