from robotBase import Subsystem
from robotBase.actuation.HBridgeMotor import PWMHBridgeMotor, MotorDirection
from robotBase.IOMap import GPIOMap
from robotBase.simulation.SimState import SimState

from Constants import Constants

class MecanumIOMap(GPIOMap):
    def __init__(self, pwmFreq: int, fl: tuple, fr: tuple, bl: tuple, br: tuple) -> None:
        super().__init__()
        self.fl = PWMHBridgeMotor(fl, pwmFreq)
        self.fr = PWMHBridgeMotor(fr, pwmFreq)
        self.bl = PWMHBridgeMotor(bl, pwmFreq)
        self.br = PWMHBridgeMotor(br, pwmFreq)
        self.modules = [self.fl, self.fr, self.bl, self.br]

        [mod.initGPIO() for mod in self.modules]

        # [self.addPin(pin) for mod in self.modules for pin in mod.pins] # Add all pins to the map
        # self.initOut() # Initialize all pins as outputs
    
    def setSpeed(self, module: int, speed: int) -> None:
        speed = max(min(speed, 1), -1) # Clamp the speed to [-1, 1]
        direction = MotorDirection.FORWARD if speed > 0 else MotorDirection.BACKWARD if speed < 0 else MotorDirection.STOP
        self.modules[module].set(direction)

    def setInverts(self, inverts: tuple[bool, bool, bool, bool]) -> None:
        [self.modules[i].setInverted(inverts[i]) for i in range(4)] # Set the inverts for each module

    def stopMod(self, module: int) -> None:
        self.modules[module].stop()

    def stop(self) -> None:
        [mod.stop() for mod in self.modules]
    
    def cleanup(self) -> None:
        [mod.cleanup() for mod in self.modules]

    @staticmethod
    def getIoPreset():
        ioMap = MecanumIOMap(Constants.GPIOMap.PWM_FREQ, Constants.GPIOMap.FL, Constants.GPIOMap.FR, Constants.GPIOMap.BL, Constants.GPIOMap.BR)
        ioMap.setInverts((Constants.GPIOMap.FL_INVERT, Constants.GPIOMap.FR_INVERT, Constants.GPIOMap.BL_INVERT, Constants.GPIOMap.BR_INVERT))
        return ioMap

class MecanumDrive(Subsystem.Subsystem):
    def __init__(self, ioMap: MecanumIOMap):
        super().__init__("DRIVETRAIN")
        self.io = ioMap

    def robotCentric(self, x, y, r) -> None:
        self.io.fl.setWithSpeed(y + r - x) # FL
        self.io.fr.setWithSpeed(y - r + x) # FR
        self.io.bl.setWithSpeed(y + r + x) # BL
        self.io.br.setWithSpeed(y - r - x) # BR
        self.telemetry.verbose(f"ROB CENT - FL: {self.io.modules[0].speed}, FR: {self.io.modules[1].speed}, BL: {self.io.modules[2].speed}, BR: {self.io.modules[3].speed}")
    
    def effectiveTank(self, left, right) -> None:
        self.io.fl.setWithSpeed(left)
        self.io.fr.setWithSpeed(right)
        self.io.bl.setWithSpeed(left)
        self.io.br.setWithSpeed(right)
        self.telemetry.verbose(f"TANK - LEFT: {self.io.modules[1].speed}, RIGHT: {self.io.modules[0].speed}")

    def fieldCentric(self, x, y, r) -> None:
        pass

    def pwmTest(self, speed) -> None:
        self.io.br.setWithSpeed(speed)
        self.telemetry.verbose(f"PWM TEST - BR: {self.io.modules[0].speed}")
    
    def stop(self) -> None:
        self.io.stop()
        self.telemetry.warn("Stopping all motors.")

    def estop(self) -> None:
        super().estop()
        self.stop()

if SimState.isSimulation():
    from robotBase.simulation.GPIOSim import GPIOSim as gpio
else:
    import RPi.GPIO as gpio # type: ignore
