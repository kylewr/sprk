import datetime
from robotBase.simulation.SimState import SimState
from robotBase.RobotEnums import RobotState

class Telemetry:
    def __init__(self) -> None:
        self._dsMethod = None

    def setDSMessageSupplier(self, sup) -> None:
        self._dsMethod = sup
    
    def sendDS(self, message: str) -> None:
        if self._dsMethod is not None:
            self._dsMethod(message)

    def getTimestamp(self) -> str:
        return datetime.datetime.now().strftime("%m/%d %H:%M:%S.%f")[:-3]

    def logRobotState(self, state: int) -> None:
        print(f"\033[94m[{self.getTimestamp()}] Robot changed state: \033[0m{RobotState(state).name}")

    def info(self, message: str) -> None:
        toSend = f"[{self.getTimestamp()}] {message}"
        print(toSend)
        if self._dsMethod is not None:
            self._dsMethod("%NL" +toSend)
    
    def success(self, message: str) -> None:
        toSend = f"[{self.getTimestamp()}] {message}"
        print(f"\033[92m{toSend}\033[0m")
        if self._dsMethod is not None:
            self._dsMethod(f"%NL%%GREEN%{toSend}")

    def warn(self, message: str) -> None:
        toSend = f"[{self.getTimestamp()}] {message}"
        print(f"\033[93m{toSend}\033[0m")
        if self._dsMethod is not None:
            self._dsMethod(f"%NL%%YELLOW%{toSend}")
    
    def err(self, message: str) -> None:
        toSend = f"[{self.getTimestamp()}] {message}"
        print(f"\033[91m{toSend}\033[0m")
        if self._dsMethod is not None:
            self._dsMethod(f"%NL%%RED%{toSend}")
    
    def logSubsystem(self, subsystem: str, message: str, color: str = "\033[0m") -> None:
        toSend = f"[{self.getTimestamp()}] ({subsystem}): {message}"
        print(f"{color}{toSend}\033[0m")
        if self._dsMethod is not None:
            colorTranslation = {
                "\033[91m": f"%RED%",
                "\033[92m": f"%GREEN%",
                "\033[93m": f"%YELLOW%",
                "\033[0m": ""
            }
            if (color in colorTranslation.keys()):
                toSend = f"{colorTranslation[color]}{toSend}"
            self._dsMethod("%NL%" + toSend)

class TelemetrySubsystem:
    def __init__(self, name: str) -> None:
        self.name = name
        self.telem = None

        self.isVerbose = False
    
    def pass_telem(self, telem: Telemetry) -> None:
        self.telem = telem

    def info(self, message: str, color: str = "\033[0m") -> None:
        if self.telem:
            self.telem.logSubsystem(self.name, message, color)
    
    def success(self, message: str) -> None:
        self.info(message, "\033[92m")

    def warn(self, message: str) -> None:
        self.info(message, "\033[93m")
    
    def err(self, message: str) -> None:
        self.info(message, "\033[91m")

    def sim(self, message: str) -> None:
        if SimState.isSimulation():
            self.info(message)
    
    def verbose(self, message: str) -> None:
        if self.telem and self.isVerbose:
            self.info(message)
    
    def toggleVerbose(self) -> None:
        self.isVerbose = not self.isVerbose
        if self.isVerbose:
            self.success("Verbose enabled.")