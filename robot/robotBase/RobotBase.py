from robotBase.Telemetry import Telemetry
from robotBase.RobotEnums import RobotState, JoystickButton
from robotBase.simulation.SimState import SimState

class RobotBase:
    def __init__(self) -> None:
        self.telemetry = Telemetry()
        self.telemetry.info("RobotBase initialized.")
        self.state = RobotState.DISABLED
        self.autonThread = None

        self.teleopInstructions: dict[JoystickButton, function] = {}
    
    def finalizeInit(self) -> None:
        self.changeState(RobotState.DISABLED)
        self.telemetry.info("Robot initialization complete!")
        if (SimState.isSimulation()):
            self.telemetry.info("Simulated robot initialization complete!")

    def changeState(self, state: RobotState) -> None:
        if (state != RobotState.AUTONOMOUS and self.autonThread is not None):
            # self.autonThread.
            self.autonThread = None

        self.state = state
        self.telemetry.logRobotState(state.value)

    # def createAutonomousThread(self, target) -> threading.Thread:
    #     return threading.Thread(target=target, args=(self,), daemon=True)

    def autonomousInit(self):
        if (self.state != RobotState.AUTONOMOUS):
            self.telemetry.sendDS(f"[STATE] AUTONOMOUS")
            self.changeState(RobotState.AUTONOMOUS)
            return True
        return False
    
    def teleopInit(self):
        if (self.state != RobotState.TELEOP):
            self.telemetry.sendDS(f"[STATE] TELEOP")
            self.changeState(RobotState.TELEOP)
            return True
        return False

    def disabledInit(self):
        if (self.state != RobotState.DISABLED):
            self.telemetry.sendDS(f"[STATE] DISABLE")
            self.changeState(RobotState.DISABLED)
            return True
        return False
        
    def emergencyStop(self) -> None:
        pass

    def handleTeleop(self, packet: str) -> None:
        if (self.state != RobotState.TELEOP):
            return
        match packet.split(",")[0]:
            case "jstk":
                dctrls = packet.replace(';', '').replace('\n', '').split(',')
                self.teleopInstructions["joystick"](dctrls)
            case "btn":
                name = packet.replace(';', '').replace('\n', '').split(",")[1].upper()
                try:
                    button = JoystickButton[name.replace("-", "_")]
                    if button in self.teleopInstructions:
                        self.teleopInstructions[button]()
                except KeyError:
                    self.telemetry.warn(f"Recieved an unknown teleop button: {packet}")
                    return
            case _:
                self.telemetry.warn(f"Recieved an unknown teleop input: {packet}")
    
    def registerButton(self, button: JoystickButton, func) -> None:
        self.teleopInstructions[button] = func
    
    def registerJoystick(self, func) -> None:
        self.teleopInstructions["joystick"] = func