from robotBase import Subsystem

class ArmIOMap:
    def __init__(self) -> None:
        pass

    @staticmethod
    def getIoPreset() -> 'ArmIOMap':
        return ArmIOMap()

class Arm(Subsystem.Subsystem):
    def __init__(self, ioMap: ArmIOMap) -> None:
        super().__init__("ARM")
        self.ioMap = ioMap
