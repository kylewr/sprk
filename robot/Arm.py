from robotBase.actuation import VirtualStepper
from robotBase import Subsystem

class ArmIOMap:
    def __init__(self, turretId: int, joint1: int, joint2: int) -> None:
        self.turretId = turretId
        self.joint1Id = joint1
        self.joint2Id = joint2
        self.turret = None
        self.joint1 = None
        self.joint2 = None
    
    def initSteppers(self) -> None:
        self.turret = VirtualStepper(self.turretId)
        self.joint1 = VirtualStepper(self.joint1Id)
        self.joint2 = VirtualStepper(self.joint2Id)

    @staticmethod
    def getIoPreset() -> 'ArmIOMap':
        return ArmIOMap(0, 1, 2)

class Arm(Subsystem.Subsystem):
    def __init__(self, ioMap: ArmIOMap) -> None:
        super().__init__("ARM")
        self.ioMap = ioMap
