from robotBase import Subsystem, IOMap
from robotBase.actuation.HBridgeMotor import HBridgeMotor, MotorDirection
from robotBase.simulation.SimState import SimState

from Constants import Constants

class MecanumIOMap(IOMap.GPIOMap):
    def __init__(self, fl: tuple, fr: tuple, bl: tuple, br: tuple) -> None:
        super().__init__()
        self.fl = HBridgeMotor(fl)
        self.fr = HBridgeMotor(fr)
        self.bl = HBridgeMotor(bl)
        self.br = HBridgeMotor(br)
        self.modules = [self.fl, self.fr, self.bl, self.br]

        [self.addPin(pin) for mod in self.modules for pin in mod.pins] # Add all pins to the map
        self.initOut() # Initialize all pins as outputs
    
    # def setSpeed(seself.lf, module: int, speed: int) -> None:
    #     speed = max(min(speed, 1), -1) # Clamp the speed to [-1, 1]

    #     high = self.modules[module][0]
    #     low = self.modules[module][1]
    #     speed *= -1 if self.inverts[module] else 1 # Invert the speed if needed
    #     self.speeds[module] = speed
    #     if speed == 0:
    #         gpio.output(high, gpio.HIGH)
    #         gpio.output(low, gpio.HIGH)
    #         return
    #     gpio.output(high, gpio.HIGH if speed < 0 else gpio.LOW)
    #     gpio.output(low, gpio.HIGH if speed > 0 else gpio.LOW)
    
    def setSpeed(self, module: int, speed: int) -> None:
        speed = max(min(speed, 1), -1) # Clamp the speed to [-1, 1]
        direction = MotorDirection.FORWARD if speed > 0 else MotorDirection.BACKWARD if speed < 0 else MotorDirection.STOP
        self.modules[module].set(direction)

    def setInverts(self, inverts: list) -> None:
        [self.modules[i].setInverted(inverts[i]) for i in range(4)] # Set the inverts for each module

    def stopMod(self, module: int) -> None:
        self.modules[module].stop()

    def stop(self) -> None:
        [self.stopMod(i) for i in range(4)]
    
    @staticmethod
    def getIoPreset():
        # return MecanumIOMap((3, 5), (11, 13), (38, 36), (37, 35))
        ioMap = MecanumIOMap(Constants.GPIOMap.FL, Constants.GPIOMap.FR, Constants.GPIOMap.BL, Constants.GPIOMap.BR)
        ioMap.setInverts((True, True, True, True))
        return ioMap


class MecanumDrive(Subsystem.Subsystem):
    def __init__(self, io: MecanumIOMap):
        super().__init__("DRIVETRAIN")
        self.ioMap = io

    def robotCentric(self, x, y, r) -> None:
        self.ioMap.setSpeed(0, y + r - x) # FL
        self.ioMap.setSpeed(1, y - r + x) # FR
        self.ioMap.setSpeed(2, y + r + x) # BL
        self.ioMap.setSpeed(3, y - r - x) # BR
        self.telemetry.verbose(f"ROB CENT - FL: {self.ioMap.modules[0].speed}, FR: {self.ioMap.modules[1].speed}, BL: {self.ioMap.modules[2].speed}, BR: {self.ioMap.modules[3].speed}")
    
    def effectiveTank(self, left, right) -> None:
        self.ioMap.setSpeed(0, left)
        self.ioMap.setSpeed(1, right)
        self.ioMap.setSpeed(2, left)
        self.ioMap.setSpeed(3, right)
        self.telemetry.verbose(f"TANK - LEFT: {self.ioMap.modules[1].speed}, RIGHT: {self.ioMap.modules[0].speed}")

    def fieldCentric(self, x, y, r) -> None:
        pass
    
    def stop(self) -> None:
        self.ioMap.stop()
        self.telemetry.warn("Stopping all motors.")

    def estop(self) -> None:
        super().estop()
        self.stop()

if SimState.isSimulation():
    from robotBase.simulation.GPIOSim import GPIOSim as gpio
    gpio.sim_setLogging(False)
else:
    import RPi.GPIO as gpio # type: ignore
