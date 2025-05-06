from enum import Enum

from robotBase.simulation.SimState import SimState

class StepperDirection(Enum):
    CW = 1  # Clockwise
    CCW = -1  # Counter-clockwise

"""
Defines a virtual stepper which goes through
an arduino to control the stepper motor.
"""
class VirtualStepper:
    def __init__(self, id: int) -> None:
        self.id = id

        self.serial: Serial = None

        self.enabled = False
        self.angle = 0.0
        self.rpm = 120
        self.continuous = False
    
    def pass_serial(self, serial) -> None:
        self.serial = serial
        self._init()
    
    def _init(self) -> None:
        if self.serial is None:
            raise Exception("Serial not set!")

    def setEnabled(self, enabled: bool) -> None:
        self.enabled = enabled
        self.serial.write(f"{self.id}e{int(enabled)}")

    def setAngle(self, angle: int) -> None:
        self.continuous = False
        self.angle = angle
        self.serial.write(f"{self.id}ang{angle}")

    def setRPM(self, rpm: int) -> None:
        self.rpm = rpm
        self.serial.write(f"{self.id}rpm{rpm}")

    def rotateContinuous(self, direction: StepperDirection) -> None:
        self.continuous = True
        if direction == StepperDirection.CW:
            self.serial.write(f"{self.id}rcw")
        elif direction == StepperDirection.CCW:
            self.serial.write(f"{self.id}rccw")

    def stop(self) -> None:
        self.continuous = False
        self.serial.write(f"{self.id}s")

if SimState.isSimulation():
    from robotBase.simulation.SerialSim import SerialSim as Serial
else:
    from robotBase.SerialBase import Serial