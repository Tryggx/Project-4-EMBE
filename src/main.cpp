#include <Arduino.h>
#include <util/delay.h>

// State Behaviour based on the C++ example at Refactoring Guru

/**
 * The base State class declares methods that all concrete states should
 * implement and also provides a backreference to the Context object, associated
 * with the State. This backreference can be used by States to transition the
 * Context to another State.
 */

#include "init_state.h"
#include "pre_op_state.h"

int timer = -1;

// Application

Context *context;

int main()
{
  init(); // Has to be included for Serial.available() to work
  context = new Context(new Init_state);
  Serial.begin(9600);

  while (true)
  {
    // wait for some time
    context->do_work();

    if (Serial.available() > 0)
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
      }
    }
  }

  delete context;
}
