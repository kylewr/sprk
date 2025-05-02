from time import sleep

from robotBase import RobotBase
from robotBase.actuation.VirtualStepper import StepperDirection
from robotBase.simulation.SimState import SimState
from robotBase.RobotState import RobotState
from robotBase.SerialBase import SerialBase as Serial

from Constants import Constants

from Arm import Arm, ArmIOMap
from MecanumDrive import MecanumDrive, MecanumIOMap
from Pinchers import Pinchers

from autonomous import DrivetrainTest

class SHARK(RobotBase.RobotBase):
    def __init__(self):
        super().__init__()

        try:
            self.serial = Serial(Constants.SERIAL_PORT, Constants.BAUD_RATE)
            self.serial.addTelemetry(self.telemetry)
            self.serial.open()
        except Exception as e:
            self.telemetry.info(f"\033[91mFailed to open serial port: {e}")
            self.telemetry.info("Falling back to simulation mode.\033[0m")
            from robotBase.simulation.SerialSim import SerialSim as SerialSim
            self.serial = SerialSim(Constants.SERIAL_PORT, Constants.BAUD_RATE)
            self.serial.addTelemetry(self.telemetry)

        self.drivetrain = MecanumDrive(MecanumIOMap.getIoPreset())
        self.drivetrain.addTelemetry(self.telemetry)

        self.arm = Arm(ArmIOMap.getIoPreset())
        self.arm.addTelemetry(self.telemetry)
        self.arm.addSerial(self.serial)

        self.pinchers = Pinchers()
        self.pinchers.addTelemetry(self.telemetry)

        self.isStopped = False

        if SimState.isSimulation():
            self.serial.telemetry.isVerbose = True
            # self.drivetrain.telemetry.isVerbose = True

        self.finalizeInit()

    def autonomousInit(self):
        if (super().autonomousInit()):
            self.autonThread = DrivetrainTest.DrivetrainTest()
            self.autonThread.passRobot(self)
            def endAction():
                sleep(0.2)
                self.disabledInit()
            self.autonThread.withEndAction(endAction)
            self.autonThread.start()

    def teleopInit(self):
        if (super().teleopInit()):
            self.drivetrain.stop()

    def disabledInit(self):
        if (self.autonThread is not None):
            self.autonThread.stop()
            self.autonThread = None

        if (super().disabledInit()):
            self.drivetrain.stop()
    
    def emergencyStop(self):
        self.drivetrain.estop()
        self.arm.estop()
        self.pinchers.estop()
        self.changeState(RobotState.DISABLED)

    def handleTeleop(self, packet: str):
        if (self.state != RobotState.TELEOP):
            return
        match packet.split(",")[0]:
            case "jstk":
                if self.isStopped:
                    self.drivetrain.stop()
                    return

                dctrls = packet.replace(";", "").replace("\n", "").split(",")
                try:
                    if dctrls[6] == '1':
                        self.drivetrain.effectiveTank(int(dctrls[2]), int(dctrls[4]))
                    else:
                        self.drivetrain.robotCentric(int(dctrls[1]), int(dctrls[2]), int(dctrls[3]))
                except ValueError:
                    self.telemetry.info(f"Invalid joystick input: {packet}")
            case "btn":
                button = packet.split(",")[1]
                # self.telemetry.info(f"Button pressed: {button}")
                if button.startswith("x;"):
                    self.arm.ioMap.turret.rotateContinuous(StepperDirection.CCW)
                elif button.startswith("b;"):
                    self.arm.ioMap.turret.rotateContinuous(StepperDirection.CW)
                elif button.startswith("a;"):
                    self.arm.ioMap.turret.stop()
                elif button.startswith("y;"):
                    self.drivetrain.telemetry.isVerbose = not self.drivetrain.telemetry.isVerbose
                elif button.startswith("leftshoulder;"):
                    self.arm.ioMap.turret.setAngle(-45)
                elif button.startswith("rightshoulder;"):
                    self.arm.ioMap.turret.setAngle(45)
            case _:
                self.telemetry.info(f"Recieved an unknown teleop input: {packet}")