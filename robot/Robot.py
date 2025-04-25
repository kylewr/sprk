import threading

from robotBase.RobotState import RobotState

from Telemetry import Telemetry
from MecanumDrive import MecanumDrive, MecanumIOMap
from Arm import Arm, ArmIOMap

class Robot:
    def __init__(self):
        self.autonThread = None

        self.telemetry = Telemetry()

        self.drivetrain = MecanumDrive(MecanumIOMap.getIoPreset())
        self.drivetrain.addTelemetry(self.telemetry)

        self.arm = Arm(ArmIOMap.getIoPreset())
        self.arm.addTelemetry(self.telemetry)

        self.changeState(RobotState.DISABLED)
        self.telemetry.info("Robot initialization complete!")

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
        # self.autonThread = self.createAutonomousThread(Simple.run)
        # self.autonThread.start()
    
    def teleopInit(self):
        if (self.state != RobotState.TELEOP):
            self.drivetrain.stop()
            self.changeState(RobotState.TELEOP)

    def disabledInit(self):
        if (self.state != RobotState.DISABLED):
            self.drivetrain.stop()
            self.changeState(RobotState.DISABLED)
    
    def emergencyStop(self):
        self.drivetrain.estop()
        self.arm.estop()
        self.changeState(RobotState.DISABLED)

