#!/bin/bash
modprobe iio-trig-hrtimer
mkdir /sys/kernel/config/iio/triggers/hrtimer/instance1
echo instance1 > /sys/bus/iio/devices/iio:device1/trigger/current_trigger
