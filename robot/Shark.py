from robotBase import RobotBase
from robotBase.RobotState import RobotState

from MecanumDrive import MecanumDrive, MecanumIOMap
from Arm import Arm, ArmIOMap

class SHARK(RobotBase.RobotBase):
    def __init__(self) -> None:
        super().__init__()

        self.drivetrain = MecanumDrive(MecanumIOMap.getIoPreset())
        self.drivetrain.addTelemetry(self.telemetry)

        self.arm = Arm(ArmIOMap.getIoPreset())
        self.arm.addTelemetry(self.telemetry)

        self.finalizeInit()

    def autonomousInit(self):
        super().autonomousInit()
        # self.autonThread = self.createAutonomousThread(Simple.run)
        # self.autonThread.start()

    def teleopInit(self):
        if (super().teleopInit()):
            self.drivetrain.stop()

    def disabledInit(self):
        if (super().disabledInit()):
            self.drivetrain.stop()
    
    def emergencyStop(self):
        self.drivetrain.estop()
        self.arm.estop()
        self.changeState(RobotState.DISABLED)

    def handleTeleop(self, packet: str) -> None:
        if (self.state != RobotState.TELEOP):
            return
        match packet.split(",")[0]:
            case "s":
                self.drivetrain.stop()
            case "jstk":
                dctrls = packet.replace(";", "").split(",")
                try:
                    if dctrls[5] == 1:
                        self.drivetrain.effectiveTank(int(dctrls[2]), int(dctrls[4]))
                    else:
                        self.drivetrain.robotCentric(int(dctrls[1]), int(dctrls[2]), int(dctrls[3]))
                except ValueError:
                    self.telemetry.info(f"Invalid joystick input: {packet}")
            case "btn":
                dctrls = packet.split(",")
            case "fc":
                pass
            case _:
                self.telemetry.info(f"Recieved an unknown teleop input: {packet}")

