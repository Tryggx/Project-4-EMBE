#define FAULT_PIN 21
#define LED_PIN 13
#define ENCODER_PIN_A 9
#define ENCODER_PIN_B 10
#define MOTOR_PIN 6
#define GEAR_RATIO 100.0
#define PPR 7
#define PI_KP 0.02
const double RPM_MAX = 105;
const double RPM_MIN = -RPM_MAX;
#include <Arduino.h>
#include <util/delay.h>
#include "digital_in.h"
#include "states/init_state.h"
#include "digital_out.h"
#include "analog_out.h"
#include "encoder.h"
#include "PI_controller.h"

// State Behaviour based on the C++ example at Refactoring Guru

/**
 * The base State class declares methods that all concrete states should
 * implement and also provides a backreference to the Context object, associated
 * with the State. This backreference can be used by States to transition the
 * Context to another State.
 */

int timer = -1;
Digital_out m2(6);
HardwareConfig hw_config = HardwareConfig(LED_PIN,
                                          MOTOR_PIN,
                                          ENCODER_PIN_A,
                                          ENCODER_PIN_B,
                                          FAULT_PIN,
                                          GEAR_RATIO,
                                          PPR,
                                          PI_KP);
// Application

Context *context;

int main()
{
  m2.init();
  m2.set_lo();
  context = new Context(new Init_state, &hw_config);
  init(); // Has to be included for Serial.available() to work
  Serial.begin(9600);

  while (true)
  {
    context->do_work();
    if (Serial.available() > 0 && context->disable_main_serial_input == false)
    {
      // read the incoming byte:
      int command = Serial.read();
      if (command == 'g')
      {
        Serial.println("Received go command");
        context->start_event();
      }
      else if (command == 's')
      {
        Serial.println("Received stop command");
        context->stop_event();
      }
      else if (command == 'r')
      {
        Serial.println("Received reset command");
        context->reset_event();
      }
      else
      {
        Serial.println("Received unknown command: ");
        Serial.println(command);
        Serial.println(millis());
      }
    }
  }

  delete context;
}
ISR(PCINT0_vect)
{
  context->io->encoder.onChange();
}

// ISR(TIMER0_COMPA_vect)
// {
//   // We use 5us as the time step, easier math and accurate enough
//   context->io->encoder.timer.micros += 5;
// }

ISR(TIMER1_COMPA_vect)
{
  OCR1B ? context->io->motor.dout.set_hi() : context->io->motor.dout.set_lo();
}

ISR(TIMER1_COMPB_vect)
{
  OCR1A > OCR1B ? context->io->motor.dout.set_lo() : context->io->motor.dout.set_hi();
}
