from robotBase.Telemetry import Telemetry
from robotBase.RobotState import RobotState
from robotBase.simulation.SimState import SimState

class RobotBase:
    def __init__(self) -> None:
        self.telemetry = Telemetry()
        self.telemetry.info("RobotBase initialized.")
        self.state = RobotState.DISABLED
        self.autonThread = None
    
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
        self.changeState(RobotState.AUTONOMOUS)
        self.telemetry.sendDS(f"[STATE] AUTONOMOUS")
        # self.autonThread = self.createAutonomousThread(Simple.run)
        # self.autonThread.start()
    
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