#!/bin/bash
#  Short script to toggle a GPIO pin at the highest frequency
#  possible using Bash - by Derek Molloy
echo 17 > /sys/class/gpio/export
echo 13 > /sys/class/gpio/export
sleep 0.5
echo "out" > /sys/class/gpio/gpio17/direction
echo "in" > /sys/class/gpio/gpio13/direction
COUNTER=0

#modified to copy the input pin: 13 to output pin: 17
while [ $COUNTER -lt 100000 ]; do
    echo `cat /sys/class/gpio/gpio13/value` > /sys/class/gpio/gpio17/value
    let COUNTER=COUNTER+1
    # unexport on ctrl-c
    # trap ctrl_c INT
    # function ctrl_c() {
    #     echo 17 > /sys/class/gpio/unexport
    #     echo 13 > /sys/class/gpio/unexport
    #     exit 0
    #}
done
echo 17 > /sys/class/gpio/unexport
