#pragma once
#include "digital_out.h"
#include "analog_out.h"
#include "encoder.h"
#include "digital_in.h"
#include "PI_controller.h"

class HardwareConfig
{
public:
  Digital_out led;
  Analog_out motor;
  Encoder encoder;
  Digital_in fault_pin;
  PI_controller controller;

  HardwareConfig(uint8_t led_pin,
                 uint8_t analog_out_pin,
                 uint8_t encoder_pinA,
                 uint8_t encoder_pinB,
                 uint8_t fault_pin_pin,
                 float gear_ratio,
                 float ppr,
                 double kp)
      : led(led_pin),
        motor(analog_out_pin),
        encoder(encoder_pinA, encoder_pinB, gear_ratio, ppr),
        fault_pin(fault_pin_pin),
        controller(3.6, 92, 0.004)
  {
  }
};
