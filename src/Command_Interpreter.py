# Motor driver for QuickBot_Follow.
# John Brewer 3/31/16
# Copyright (C) 2016 Jera Design LLC
# All Rights Reserverd

import Motor_Driver

import sys
from time import sleep

Motor_Driver.init_pins()

print "Ready"

last = ""
count = 0

while True:
    line = sys.stdin.readline().rstrip()
    if not line:
        break;
    if line == "left":
        print "left"
        if last != "left":
            count = 10
            last = "left"
            continue
        if count > 0:
            count = count - 1
            continue
        Motor_Driver.move(-90, 88, 0.01)
    if line == "right":
        print "right"
        if last != "right":
            count = 10
            last = "right"
            continue
        if count > 0:
            count = count - 1
            continue
        Motor_Driver.move(90, -88, 0.01)
    if line == "forward":
        print "forward"
        last = "forward"
        Motor_Driver.move(70, 68, 0.05)
    else:
        print "STOP"
#        last = "STOP"
