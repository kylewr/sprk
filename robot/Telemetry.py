import datetime
from robotBase.simulation.SimState import SimState
from robotBase.RobotState import RobotState

class Telemetry:
    def __init__(self):
        pass

    def getTimestamp(self) -> str:
        return datetime.datetime.now().strftime("%m/%d %H:%M:%S.%f")[:-3]

    def logRobotState(self, state: int) -> None:
        print(f"\033[94m[{self.getTimestamp()}] Robot changed state: \033[0m{RobotState(state).name}")

    def info(self, message: str) -> None:
        print(f"[{self.getTimestamp()}] {message}")
    
    def logSubsystem(self, subsystem: str, message: str, color: str = "\033[0m") -> None:
        print(f"{color}[{self.getTimestamp()}] ({subsystem}): {message}\033[0m")

class TelemetrySubsystem:
    def __init__(self, name: str) -> None:
        self.name = name
        self.telem = None
    
    def pass_telem(self, telem: Telemetry) -> None:
        self.telem = telem

    def info(self, message: str, color: str = "\033[0m") -> None:
        if self.telem:
            self.telem.logSubsystem(self.name, message, color)
    
    def warn(self, message: str) -> None:
        self.info(message, "\033[93m")
    
    def err(self, message: str) -> None:
        self.info(message, "\033[91m")

    def sim(self, message: str) -> None:
        if SimState.isSimulation():
            self.info(message)
    