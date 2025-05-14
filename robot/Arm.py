from robotBase import Subsystem
from robotBase.simulation.SimState import SimState
from robotBase.actuation.VirtualStepper import StepperDirection, VirtualStepper

from Constants import Constants

class ArmIOMap:
    def __init__(self, turretId: int, arm: int, wrist: int) -> None:
        self.turretId = turretId
        self.armId = arm
        self.wristId = wrist
        self.turret = None
        self.arm = None
        self.wrist = None
        self.initSteppers()
    
    def initSteppers(self) -> None:
        self.turret = VirtualStepper(self.turretId)
        self.arm = VirtualStepper(self.armId)
        self.wrist = VirtualStepper(self.wristId)

    @staticmethod
    def getIoPreset() -> 'ArmIOMap':
        return ArmIOMap(Constants.SerialMap.TURRET, Constants.SerialMap.ARM, Constants.SerialMap.WRIST)

class Arm(Subsystem.Subsystem):
    def __init__(self, ioMap: ArmIOMap) -> None:
        super().__init__("ARM")
        self.io = ioMap
        self.serial: Serial = None

    def addSerial(self, serial: 'Serial') -> None:
        self.serial = serial
        self.io.turret.pass_serial(serial)
        self.io.arm.pass_serial(serial)
        self.io.wrist.pass_serial(serial)

    def stow(self) -> None:
        self.setAll(Constants.ActuationConstants.POSITION_STOW)
        self.telemetry.verbose("POSITION - STOW")

    def setAll(self, angles: tuple[float, float, float]) -> None:
        self.serial.startMultiCommand()
        self.io.turret.setAngle(angles[0])
        self.io.arm.setAngle(angles[1])
        self.io.wrist.setAngle(angles[2])
        self.serial.write("\n")

    def stop(self) -> None:
        self.serial.startMultiCommand()
        self.io.turret.stop()
        self.io.arm.stop()
        self.io.wrist.stop()
        self.serial.write("\n")
        # self.serial.write(",2")
        self.telemetry.verbose("Stopping all motors.")
    
    def enable(self) -> None:
        self.serial.write(",0")
        self.telemetry.verbose("Enabling all steppers.")

    def disable(self) -> None:
        self.serial.write(",1")
        self.telemetry.warn("Disabling all steppers.")
    
    def moveArm(self, dir):
        self.serial.startMultiCommand()
        self.io.arm.rotateContinuous(dir)
        self.io.wrist.setRPM(80)
        self.io.wrist.rotateContinuous(StepperDirection.flipDir(dir))
        # self.io.wrist.rotateContinuous(dir)
        self.serial.write()

if SimState.isSimulation():
    from robotBase.simulation.SerialSim import SerialSim as Serial
else:
    from robotBase.SerialBase import Serial