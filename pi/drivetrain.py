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
        if speed == 0:
            gpio.output(high, gpio.HIGH)
            gpio.output(low, gpio.HIGH)
            return
        gpio.output(high, gpio.HIGH if speed < 0 else gpio.LOW)
        gpio.output(low, gpio.HIGH if speed > 0 else gpio.LOW)

    def stopMod(self, module: int) -> None:
        self.setSpeed(module, 0)
    def stop(self) -> None:
        [self.stopMod(i) for i in range(4)]


class MecanumDrive:
    def __init__(self, io: MecanumIOMap):
        self.ioMap = io
    
    def drive(self, x, y, r) -> None:
        self.ioMap.setSpeed(0, y + r - x) # FL
        self.ioMap.setSpeed(1, y - r + x) # FR
        self.ioMap.setSpeed(2, y + r + x) # BL
        self.ioMap.setSpeed(3, y - r - x) # BR
    
    def stop(self) -> None:
        self.ioMap.stop()


gpio.setmode(gpio.BOARD)

ioMap = MecanumIOMap(3, 5, 11, 13, 37, 35, 38, 36)
drive = MecanumDrive(ioMap)

if __name__ == '__main__':
    # try :
    while True:
        drive.stop()
        sleep(1)

        drive.drive(0, 1, 0)
        print("forward")
        sleep(1)
        drive.stop()
        sleep(0.5)
        drive.drive(0, -1, 0)
        print("backward")
        sleep(2)
        drive.stop()
        sleep(0.5)
        drive.drive(0, 1, 0)
        print("forward")
        sleep(1)

        drive.stop()
        sleep(1)

        drive.drive(-1, 0, 0)
        print("left")
        sleep(1)
        drive.stop()
        sleep(0.5)
        drive.drive(1, 0, 0)
        print("right")
        sleep(2)
        drive.stop()
        sleep(0.5)
        drive.drive(-1, 0, 0)
        print("left")
        sleep(1)

        drive.stop()
        sleep(1)

        drive.drive(0, 0, 1)
        print("rotate right")
        sleep(2)
        drive.stop()
        sleep(0.5)
        drive.drive(0, 0, -1)
        print("rotate left")
        sleep(2)
        
        
    # except:
    #     drive.stop()