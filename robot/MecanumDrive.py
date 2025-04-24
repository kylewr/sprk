from robotBase import Subsystem, IOMap
from simulation.SimState import SimState

class MecanumIOMap(IOMap.GPIOMap):
    def __init__(self, fl: tuple, fr: tuple, bl: tuple, br: tuple) -> None:
        super().__init__()
        self.fl = fl
        self.fr = fr
        self.bl = bl
        self.br = br
        self.modules = [fl, fr, bl, br]
        self.inverts = [False, False, False, False]

        [self.addPin(pin) for mod in self.modules for pin in mod] # Add all pins to the map
        self.initOut() # Initialize all pins as outputs
    
    def setSpeed(self, module: int, speed: int) -> None:
        speed = max(min(speed, 1), -1) # Clamp the speed to [-1, 1]

        high = self.modules[module][0]
        low = self.modules[module][1]
        if speed == 0:
            gpio.output(high, gpio.HIGH)
            gpio.output(low, gpio.HIGH)
            return
        speed *= -1 if self.inverts[module] else 1 # Invert the speed if needed
        gpio.output(high, gpio.HIGH if speed < 0 else gpio.LOW)
        gpio.output(low, gpio.HIGH if speed > 0 else gpio.LOW)
    
    def setInverts(self, inverts: list) -> None:
        self.inverts = inverts

    def stopMod(self, module: int) -> None:
        self.setSpeed(module, 0)
    def stop(self) -> None:
        [self.stopMod(i) for i in range(4)]
    
    @staticmethod
    def getIoPreset():
        return MecanumIOMap((3, 5), (11, 13), (38, 36), (37, 35))


class MecanumDrive(Subsystem.Subsystem):
    def __init__(self, io: MecanumIOMap):
        super().__init__("DRIVETRAIN")
        self.ioMap = io

    def robotCentric(self, x, y, r) -> None:
        self.ioMap.setSpeed(0, y + r - x) # FL
        self.ioMap.setSpeed(1, y - r + x) # FR
        self.ioMap.setSpeed(2, y + r + x) # BL
        self.ioMap.setSpeed(3, y - r - x) # BR
        self.telemetry.sim(f"FL: {y + r - x}, FR: {y - r + x}, BL: {y + r + x}, BR: {y - r - x}")
    
    def fieldCentric(self, x, y, r) -> None:
        pass
    
    def stop(self) -> None:
        self.ioMap.stop()
        self.telemetry.sim("Stopping all motors.")

if SimState.isSimulation():
    from simulation.GPIOSim import GPIOSim as gpio
    gpio.sim_setLogging(False)
else:
    import RPi.GPIO as gpio # type: ignore
