#include <Arduino.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

double last_control_time = 0.0;
gearRatio = 100;
pulsesPerRev = 7;

void on_do(double val)
{

  if (micros() - last_control_time > 4000)
  {
    double last_control_time = micros();
    // calculate rpm from encoder count
    // 7 pulses per revolution, gear ratio 100, 60 seconds per minute
    float tempRpm = 1*1000000 / ((micros() - val) * pulsesPerRev * gearRatio * 60);
    


    //double actual = this->context_->io->encoder.getRpm();
    //double output = this->context_->io->controller.update(ref_rpm, actual);
    //this->motor_pwm = output / 105;
    //this->context_->io->motor.set(this->motor_pwm);
    //print csv, Actual rpm, requested rpm and pwm and millis
    // Serial.print(actual);
    // Serial.print(",");
    // Serial.print(ref_rpm);
    // Serial.print(",");
    // Serial.print(this->motor_pwm);
    // Serial.print(",");
    // Serial.println(millis());
  }
  // if (millis() % 3000 < 30){
  //   ref_rpm == 50.0 ? ref_rpm = 90.0 : ref_rpm = 50.0;
  // }
}

void ReadKernel()
{
    while (1)
    {
        int fd = open("/dev/encoderCounter", O_RDONLY);
        double val = read(fd, &c, 1);
        on_do(val);
        close(fd);
        printf("Read val: Read var is %d\n", val);
        usleep(1000000);
    }
}