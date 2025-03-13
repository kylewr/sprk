import RPi.GPIO as gpio
import keyboard
from time import sleep

import tty
import sys
import termios

orig_settings = termios.tcgetattr(sys.stdin)

tty.setcbreak(sys.stdin)

class MecanumIOMap:
    def __init__(self, fl_h, fl_l, fr_h, fr_l, br_h, br_l, bl_h, bl_l):
        self.fl_h = fl_h
        self.fl_l = fl_l
        self.fr_h = fr_h
        self.fr_l = fr_l
        self.br_h = br_h
        self.br_l = br_l
        self.bl_h = bl_h
        self.bl_l = bl_l
        self.all = [fl_h, fl_l, fr_h, fr_l, br_h, br_l, bl_h, bl_l]
        self.initOut()
    
    def initOut(self) -> None:
        [gpio.setup(pin, gpio.OUT) for pin in self.all]
    
    def setSpeed(self, module: int, speed: int) -> None:
        high = self.all[module * 2]
        low = self.all[module * 2 + 1]
        gpio.output(high, gpio.HIGH if speed > 0 else gpio.LOW)
        gpio.output(low, gpio.LOW if speed < 0 else gpio.HIGH)

    def stopModule(self, module: int) -> None:
        self.setSpeed(module, 0)
    def stop(self) -> None:
        [self.stopModule(i) for i in range(4)]
        


class MecanumDrive:
    def __init__(self, io: MecanumIOMap):
        self.ioMap = io
        self.isStopped = True
    
    def drive(self, x, y, r) -> None:
        self.ioMap.setSpeed(0, y + x + r)
        self.ioMap.setSpeed(1, y - x - r)
        self.ioMap.setSpeed(2, y + x - r)
        self.ioMap.setSpeed(3, y - x + r)
        self.log(x, y, r)
        self.isStopped = (abs(x) + abs(y) + abs(r)) == 0
    
    def log(self, x, y, r) -> None:
        print(f"X: {x}, Y: {y}, R: {r}")

    def stop(self) -> None:
        self.ioMap.stop()
        self.log(0, 0, 0)
        self.isStopped = True


gpio.setmode(gpio.BCM)

ioMap = MecanumIOMap(5, 6, 13, 19, 4, 17, 27, 2)
drive = MecanumDrive(ioMap)

x = 0;
y = 0;
r = 0;

print("DRIVE!")

cha = 0

try:
    while True:
        while cha != chr(27): # ESC
            cha=sys.stdin.read(1)[0]
            print("You pressed", cha)
            b = False;
            if (keyboard.is_pressed('w')):
                y += 1
                b = True
            if (keyboard.is_pressed('s')):
                y -= 1
                b = True
            if (keyboard.is_pressed('a')):
                x += 1
                b = True
            if (keyboard.is_pressed('d')):
                x -= 1
                b = True
            
            if b: break

        if (not drive.isStopped):
            drive.drive(x, y, r)

        x = 0
        y = 0
        r = 0

        sleep(0.05)
except KeyboardInterrupt:
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, orig_settings)
    drive.stop()
    gpio.cleanup()
