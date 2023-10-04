#ifndef STATE_BASE_H
#define STATE_BASE_H

#include <Arduino.h>
#include "hwconfig.h"
class Context; // Forward declaration to allow the definition of a pointer to Context without compile error
class State
{
  /**
   * @var Context
   */

protected:
  Context *context_;

public:
  virtual ~State()
  {
  }

  void set_context(Context *context)
  {
    this->context_ = context;
  }

  virtual void on_do() = 0;

  virtual void on_entry() = 0;

  virtual void on_exit() = 0;

  virtual void on_reset() = 0;

  virtual void on_start() = 0;

  virtual void on_set_pre_operational() = 0;

  virtual void on_stop() = 0;
  // ...
};

class Context
{
  /**
   * @var State A reference to the current state of the Context.
   */

private:
  State *state_;
public:
  HardwareConfig *io;
  Context(State *state, HardwareConfig *hwconfig) : state_(nullptr)
  {
    this->transition_to(state);
    this->io = hwconfig;
  }

  ~Context()
  {
    delete state_;
  }

  /**
   * The Context allows changing the State object at runtime.
   */

  void transition_to(State *state)
  {
    if (this->state_ != nullptr)
    {
      this->state_->on_exit();
      delete this->state_;
    }

    this->state_ = state;

    this->state_->set_context(this);

    this->state_->on_entry();
  }

  /**
   * The Context delegates part of its behavior to the current State object.
   */

  void do_work()
  {
    this->state_->on_do();
  }

  void start_event()
  {
    this->state_->on_start();
  }

  void stop_event()
  {
    this->state_->on_stop();
  }
  void reset_event()
  {
    this->state_->on_reset();
  }
  void pre_op_event()
  {
    this->state_->on_set_pre_operational();
  }
};

#endif // STATE_BASE_H