# Motor driver for QuickBot_Follow.
# John Brewer 3/31/16
# Copyright (C) 2016 Jera Design LLC
# All Rights Reserverd

import Motor_Driver

import sys

Motor_Driver.init_pins()

print "Ready"

while True:
    line = sys.stdin.readline().rstrip()
    if not line:
        break;
    if line == "left":
        print "turn left"
        Motor_Driver.move(-70, 70, 0.05)
    elif line == "right":
        print "turn right"
        Motor_Driver.move(70, -70, 0.05)
    elif line == "straight":
        print "go straight"
        Motor_Driver.move(100, 98, .95)
    else:
        print "STOP"
