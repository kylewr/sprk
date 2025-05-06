from robotBase import Subsystem
from robotBase.simulation.SimState import SimState
from robotBase.actuation.VirtualStepper import VirtualStepper

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

    def addSerial(self, serial) -> None:
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
        self.telemetry.warn("Stopping all motors.")

if SimState.isSimulation():
    from robotBase.simulation.SerialSim import SerialSim as Serial
else:
    from robotBase.SerialBase import Serial