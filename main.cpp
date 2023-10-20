#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include "PI_controller.h"

int first_reading = 0;
int second_reading = 0;
int first_reading_time = 0;
int second_reading_time = 0;
int last_control_time = 0;
double gearRatio = 100;
double pulsesPerRev = 7;
int time_between_reads = 50000;
char pwm_pin[3] = "12";
int motor_pin1 = 23;
int motor_pin2 = 24;
int read_encoder()
{
    while (1)
    {
        char i[10];
        int fd = open("/dev/encoderCounter", O_RDONLY);
        int val = read(fd, i, 10);
        //val should be string, convert to int
        int intval = atoi(i);
        close(fd);
        return intval;
    }
}

int main()
{
  PI_controller pi = PI_controller(1.6, 0.2, 0.05);
  double ref_rpm = 50.0;
  int fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
  write(fd, "0", 1);
  close(fd);
  //set pin pwmpin to output
  fd = open("/sys/class/gpio/unexport", O_WRONLY);
  //use pwm_pin
  write(fd, pwm_pin, 2);
  close(fd);
  // fd = open("/sys/class/gpio/gpio13/direction", O_WRONLY);
  // write(fd, "out", 3);
  // close(fd);
  //Set period to 100000
  fd = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
  write(fd, "100000", 6);
  close(fd);
  //Set duty cycle to 50000
  fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
  write(fd, "50000", 5);
  close(fd);
  //enable pwm
  fd = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
  write(fd, "1", 1);
  close(fd);



  fd  = open("/sys/class/gpio/export", O_WRONLY);
  char motor_pin2_string[3];
  sprintf(motor_pin2_string, "%d", motor_pin2);
  write(fd, motor_pin2_string, 2);
  close(fd);

  fd = open("/sys/class/gpio/gpio24/direction", O_WRONLY);
  write(fd, "out", 3);
  close(fd);

  fd = open("/sys/class/gpio/gpio24/value", O_WRONLY);
  write(fd, "0", 1);
  close(fd);

  
  while(1)
  {
    //usleep(4000);
    first_reading = read_encoder();
    usleep(time_between_reads);
    second_reading = read_encoder();
    printf("First reading: %d\n", first_reading);
    printf("Second reading: %d\n", second_reading);
    //calculate rpm from encoder count
    // 7 pulses per revolution, gear ratio 100, 60 seconds per minute
    //pulses should be 7*100 times per rev
    int pulses = second_reading - first_reading;
    double pulses_per_second = pulses / (time_between_reads / 1000000.0);
    double current_rpm = (pulses_per_second * 60.0) / (pulsesPerRev * gearRatio);
    double pi_output = pi.update(ref_rpm, current_rpm);
    int fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
    //pi_output is new rpm, divide by 120
    double new_rpm = (pi_output/120.0) * 100000.0;
    char pi_output_string[10];
    printf("New RPM: %f\n", new_rpm);
    if (new_rpm > 100000.0){
      new_rpm = 100000.0;
    }
    sprintf(pi_output_string, "%d", (int)new_rpm);
    write(fd, pi_output_string, 6);
    close(fd);
    printf("Current RPM: %f\n", current_rpm);   
  }
  // if (millis() % 3000 < 30){
  //   ref_rpm == 50.0 ? ref_rpm = 90.0 : ref_rpm = 50.0;
  // }
}

