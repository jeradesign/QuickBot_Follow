# Motor driver for QuickBot_Follow.
# John Brewer 3/31/16
# Copyright (C) 2016 Jera Design LLC
# All Rights Reserverd

import Adafruit_BBIO.GPIO as GPIO
import Adafruit_BBIO.PWM as PWM
from time import sleep
 
GPIO.setup("P8_12", GPIO.OUT)
GPIO.output("P8_12", GPIO.LOW)
GPIO.setup("P8_10", GPIO.OUT)
GPIO.output("P8_10", GPIO.LOW)

GPIO.setup("P8_14", GPIO.OUT)
GPIO.output("P8_14", GPIO.LOW)
GPIO.setup("P8_16", GPIO.OUT)
GPIO.output("P8_16", GPIO.LOW)

PWM.start("P9_14", 0)
PWM.start("P9_16", 0)

GPIO.cleanup()

GPIO.output("P8_12", GPIO.LOW)
GPIO.output("P8_10", GPIO.HIGH)
PWM.set_duty_cycle("P9_14", 50)

sleep(1)

PWM.set_duty_cycle("P9_14", 0)
GPIO.output("P8_12", GPIO.LOW)
GPIO.output("P8_10", GPIO.LOW)
