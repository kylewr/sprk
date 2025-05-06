from time import sleep

from robotBase import RobotBase
from robotBase.actuation.VirtualStepper import StepperDirection
from robotBase.simulation.SimState import SimState
from robotBase.RobotEnums import RobotState, JoystickButton, JoystickAxis
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
            self.telemetry.err(f"Failed to open serial port: {e}")
            self.telemetry.err("Falling back to simulation mode.")
            from robotBase.simulation.SerialSim import SerialSim as SerialSim
            self.serial = SerialSim(Constants.SERIAL_PORT, Constants.BAUD_RATE)
            self.serial.addTelemetry(self.telemetry)

        if SimState.isSimulation():
            from robotBase.simulation.GPIOSim import GPIOSim
            GPIOSim.addTelemetry(self.telemetry)
            GPIOSim.telemetry.isVerbose = False

            self.serial.telemetry.isVerbose = True

        self.drivetrain = MecanumDrive(MecanumIOMap.getIoPreset())
        self.drivetrain.addTelemetry(self.telemetry)

        self.arm = Arm(ArmIOMap.getIoPreset())
        self.arm.addTelemetry(self.telemetry)
        self.arm.addSerial(self.serial)

        self.pinchers = Pinchers(Constants.GPIOMap.SERVO)
        self.pinchers.addTelemetry(self.telemetry)

        if SimState.isSimulation():
            self.arm.telemetry.isVerbose = True
            self.pinchers.telemetry.isVerbose = True

        self.registerJoystickCallback(self._joystickFunc)
        self._loadButtons()

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
            self.arm.stop()
            self.pinchers.stop()
    
    def emergencyStop(self):
        self.drivetrain.estop()
        self.arm.estop()
        self.pinchers.estop()
        self.changeState(RobotState.DISABLED)

    def _joystickFunc(self, jctrls: dict['JoystickAxis', int]):
        self.drivetrain.robotCentric(jctrls[JoystickAxis.LEFT_X], jctrls[JoystickAxis.LEFT_Y], jctrls[JoystickAxis.RIGHT_TRIGGER] - jctrls[JoystickAxis.LEFT_TRIGGER])

    def _loadButtons(self):
        self.registerButton(JoystickButton.A, self.pinchers.open)
        self.registerButton(JoystickButton._A, self.pinchers.close)

        self.registerButton(JoystickButton.RIGHTSHOULDER, self.arm.stow)

        self.registerButton(JoystickButton._BACK, self.drivetrain.telemetry.toggleVerbose)