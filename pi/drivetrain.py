import RPi.GPIO as gpio
from time import sleep

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
    
    def drive(self, x, y, r) -> None:
        self.ioMap.setSpeed(0, y + x + r)
        self.ioMap.setSpeed(1, y - x - r)
        self.ioMap.setSpeed(2, y + x - r)
        self.ioMap.setSpeed(3, y - x + r)
        self.log(x, y, r)
    
    def log(self, x, y, r) -> None:
        print(f"X: {x}, Y: {y}, R: {r}")

    def stop(self) -> None:
        self.ioMap.stop()


gpio.setmode(gpio.BCM)

ioMap = MecanumIOMap(5, 6, 13, 19, 4, 17, 27, 2)
drive = MecanumDrive(ioMap)

while True:
    drive.stop()
    sleep(1)
    drive.drive(1, 0, 0)
    sleep(1)