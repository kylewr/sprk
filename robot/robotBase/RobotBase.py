from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from robotBase.AutonomousThread import AutonomousThread

from threading import Thread
from time import sleep

from robotBase.Telemetry import Telemetry
from robotBase.RobotEnums import RobotState, JoystickButton, JoystickAxis
from robotBase.simulation.SimState import SimState

class RobotBase:
    def __init__(self) -> None:
        self.telemetry = Telemetry()
        self.telemetry.info("RobotBase initialized.")
        self.state = RobotState.DISABLED

        self.alive = True
        self.timeSinceLastPacket = 0
        self.useHeartbeatCheck = True
        self._heartbeatThreadGenerator = lambda: Thread(target=self._heartbeatTarget, daemon=True)
        self._heartbeatThread = None

        self._autonMap: dict[str, AutonomousThread] = {}
        self._selectedAutonName: str = None
        self._selectedAutonThread: AutonomousThread = None

        self.teleopInstructions: dict[JoystickButton, function] = {}
    
    def _heartbeatTarget(self) -> None:
        self.timeSinceLastPacket = 0
        while self.alive and self.state != RobotState.DISABLED and self.useHeartbeatCheck:
            sleep(0.5)
            self.timeSinceLastPacket += 1
            if self.timeSinceLastPacket > 1:
                self.disabledInit()
                self.telemetry.err("Heartbeat lost! Disabling robot.")

    def startHeartbeat(self) -> None:
        self._heartbeatThread = self._heartbeatThreadGenerator()
        self._heartbeatThread.start()

    def finalizeInit(self) -> None:
        self.changeState(RobotState.DISABLED)
        self.telemetry.info("Robot initialization complete!")
        if (SimState.isSimulation()):
            self.telemetry.info("Simulated robot initialization complete!")

    def changeState(self, state: RobotState) -> None:
        if (state != RobotState.AUTONOMOUS and self._selectedAutonThread is not None):
            # self.autonThread.
            self._selectedAutonThread = None

        self.state = state
        self.telemetry.logRobotState(state.value)

    def getAutonsAsCSV(self):
        return ','.join(self._autonMap.keys())

    def autonomousInit(self):
        if (self.state != RobotState.AUTONOMOUS):
            if self._selectedAutonName is None:
                self.telemetry.err("Failed to enable: No autonomous selected!")
                return
            self.telemetry.sendDS(f"[STATE] AUTONOMOUS")
            self.changeState(RobotState.AUTONOMOUS)

            return True
        return False
    
    def teleopInit(self):
        if (self.state != RobotState.TELEOP):
            self.useHeartbeatCheck = True
            self.telemetry.sendDS(f"[STATE] TELEOP")
            self.changeState(RobotState.TELEOP)
            self.startHeartbeat()
            return True
        return False

    def disabledInit(self):
        self.telemetry.sendDS(f"[STATE] DISABLE")
        if (self.state != RobotState.DISABLED):
            self.useHeartbeatCheck = False
            self.changeState(RobotState.DISABLED)

            if (self._heartbeatThread is not None):
                try:
                    self._heartbeatThread.join()
                except RuntimeError:
                    pass
                self._heartbeatThread = None
            return True
        return False
        
    def emergencyStop(self) -> None:
        pass

    def cleanup(self) -> None:
        self.alive = False
        self.telemetry.info("RobotBase cleanup complete.")
        if (SimState.isSimulation()):
            self.telemetry.info("Simulated robot cleanup complete.")
        if (self._heartbeatThread is not None):
            self._heartbeatThread.join()
        self._heartbeatThread = None

    def handleTeleop(self, packet: str) -> None:
        if (self.state != RobotState.TELEOP):
            return
        match packet.split(",")[0]:
            case "jstk":
                dctrls = packet.replace(';', '').replace('\n', '').split(',')[1:]
                try:
                    self.teleopInstructions["joystick"](JoystickAxis.convertFromList(dctrls))
                except ValueError:
                    self.telemetry.warn(f"Failed to decode joystick axies: {dctrls}")
                    return
            case "btn":
                name = packet.replace(';', '').replace('\n', '').split(",")[1].upper()
                try:
                    button = JoystickButton[name.replace("-", "_")]
                    if button in self.teleopInstructions:
                        self.teleopInstructions[button]()
                except KeyError:
                    self.telemetry.warn(f"Recieved an unknown teleop button: {name}")
                    return
            case _:
                self.telemetry.warn(f"Recieved an unknown teleop input: {packet}")
    
    def registerButton(self, button: JoystickButton, func) -> None:
        self.teleopInstructions[button] = func
    
    def registerJoystickCallback(self, func) -> None:
        self.teleopInstructions["joystick"] = func