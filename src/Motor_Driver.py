# Motor driver for QuickBot_Follow.
# John Brewer 3/31/16
# Copyright (C) 2016 Jera Design LLC
# All Rights Reserverd

import Adafruit_BBIO.GPIO as GPIO
import Adafruit_BBIO.PWM as PWM
from time import sleep
 
def init_pins():
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

def move(leftPercent, rightPercent, duration):
    if (leftPercent > 0):
        GPIO.output("P8_14", GPIO.LOW)
        GPIO.output("P8_16", GPIO.HIGH)
    else:
        GPIO.output("P8_14", GPIO.HIGH)
        GPIO.output("P8_16", GPIO.LOW)

    if (rightPercent > 0):
        GPIO.output("P8_12", GPIO.LOW)
        GPIO.output("P8_10", GPIO.HIGH)
    else:
        GPIO.output("P8_12", GPIO.HIGH)
        GPIO.output("P8_10", GPIO.LOW)
        
    PWM.set_duty_cycle("P9_16", abs(leftPercent))
    PWM.set_duty_cycle("P9_14", abs(rightPercent))

    sleep(duration);

    GPIO.output("P8_12", GPIO.LOW)
    GPIO.output("P8_10", GPIO.LOW)

    GPIO.output("P8_14", GPIO.LOW)
    GPIO.output("P8_16", GPIO.LOW)

    PWM.set_duty_cycle("P9_16", 0)
    PWM.set_duty_cycle("P9_14", 0)

try:
    init_pins()
    move(100, 98, 2)
    move(-70, 70, 0.5)
    move(100, 98, 2)
    move(-70, 70, 0.5)
finally:
    GPIO.cleanup()

