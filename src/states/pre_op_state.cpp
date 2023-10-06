#include "pre_op_state.h"
#include "init_state.h"
#include "op_state.h"

void Pre_op_state::on_do()
{

  if (millis() - this->last_time > 500)
  {
    this->last_time = millis();
    this->context_->io->led.toggle();
  }

  if (this->current_step == 0)
  {
    if (this->first_run)
    {
      this->context_->disable_main_serial_input = true;
      Serial.println("Enter value for Kp:");
      this->first_run = false;
      Serial.flush();
      this->input_buffer_index = 0;
    }
    if (Serial.available() > 0)
    {
      char current = Serial.read();
      Serial.print(current);
      if (current == '\n')
      {
        this->input_buffer[this->input_buffer_index] = '\0';
        this->context_->io->controller.set_Kp(atof(this->input_buffer));
        this->input_buffer_index = 0;
        this->current_step++;
        this->first_run = true;
      }
      else
      {
        this->input_buffer[this->input_buffer_index] = current;
        this->input_buffer_index++;
      }
    }
  }
  if (this->current_step == 1)
  {
    if (this->first_run)
    {
      Serial.println("Enter value for Ti:");
      this->first_run = false;
      Serial.flush();
      this->input_buffer_index = 0;
    }
    if (Serial.available() > 0)
    {
      char current = Serial.read();
      Serial.print(current);
      if (current == '\n')
      {
        this->input_buffer[this->input_buffer_index] = '\0';
        this->context_->io->controller.set_Ti(atof(this->input_buffer));
        this->input_buffer_index = 0;
        this->current_step++;
      }
      else
      {
        this->input_buffer[this->input_buffer_index] = current;
        this->input_buffer_index++;
      }
    }
  }
  if (this->current_step == 2)
  {
    this->context_->disable_main_serial_input = false;
    this->context_->transition_to(new Op_state);
  }
}

void Pre_op_state::on_start()
{
}

void Pre_op_state::on_entry()
{
  Serial.println("Now in pre-op state");
  this->current_step = 0;
}

void Pre_op_state::on_reset()
{
  Serial.println("Resetting");
  this->context_->transition_to(new Init_state);
}