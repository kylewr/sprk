from time import sleep

from robotBase import RobotBase
from robotBase.actuation.VirtualStepper import StepperDirection
from robotBase.simulation.SimState import SimState
from robotBase.AutonomousThread import AutonomousThread
from robotBase.RobotEnums import RobotState, JoystickButton, JoystickAxis
from robotBase.SerialBase import SerialBase as Serial

from Camera import RobotCamera
from Constants import Constants

from Arm import Arm, ArmIOMap
from MecanumDrive import MecanumDrive, MecanumIOMap
from Pinchers import Pinchers

import autonomous as Autonomous

class SHARK(RobotBase.RobotBase):
    def __init__(self):
        super().__init__()

        try:
            self.serial = Serial(Constants.SIM_SERIAL_PORT if SimState.isSimulation() else Constants.SERIAL_PORT, Constants.BAUD_RATE)
            self.serial.addTelemetry(self.telemetry)
            self.serial.open()
        except Exception as e:
            self.telemetry.err(f"Failed to open serial port: {e}")
            self.telemetry.err("Falling back to simulation mode.")
            from robotBase.simulation.SerialSim import SerialSim as SerialSim
            self.serial = SerialSim(Constants.SIM_SERIAL_PORT, Constants.BAUD_RATE)
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

        self._autonMap: dict[str, AutonomousThread] = {
            "Very Cool Drive Demo": Autonomous.CoolDrive,
            "Drivetrain Demo": Autonomous.Demo,
            "Stepper Test": Autonomous.StepperTest,
            "Drivetrain Self-Test": Autonomous.DrivetrainTest,
        }

        self.camera = RobotCamera()
        # self.camera.start()

        self.registerJoystickCallback(self._joystickFunc)
        self._loadButtons()

        self.turretVel = 0

        self.finalizeInit()
    
    def selectAuton(self, auton: str):
        if (self.state == RobotState.AUTONOMOUS):
            return
        
        auton = auton.replace("\n", "")
        if auton in self._autonMap.keys():
            self._selectedAutonName = auton
            self.telemetry.success(f"Selected autonomous: {self._selectedAutonName}")
        else:
            self.telemetry.err(f"Invalid autonomous selected: {auton}")

    def autonomousInit(self):
        if (super().autonomousInit()):
            def endAction():
                sleep(0.2)
                self.disabledInit()

            self._selectedAutonThread = self._autonMap[self._selectedAutonName]()
            self._selectedAutonThread.passRobot(self)
            self._selectedAutonThread.withEndAction(endAction)
            self._selectedAutonThread.start()

    def teleopInit(self):
        if (self._selectedAutonThread is not None):
            self._selectedAutonThread.stop()
            self._selectedAutonThread = None

        if (super().teleopInit()):
            self.drivetrain.stop()
            self.arm.enable()

    def disabledInit(self):
        if (self._selectedAutonThread is not None):
            self._selectedAutonThread.stop()
            self._selectedAutonThread = None

        if (super().disabledInit()):
            self.drivetrain.stop()
            self.arm.disable()
            self.pinchers.stop()
    
    def emergencyStop(self):
        super().emergencyStop()
        self.drivetrain.estop()
        self.arm.estop()
        self.pinchers.estop()
        self.changeState(RobotState.DISABLED)
    
    def cleanup(self):
        super().cleanup()
        self.drivetrain.io.cleanup()
        self.camera.stop()

    def squareJoystick(self, jctrls: dict['JoystickAxis', int]):
        jctrls[JoystickAxis.LEFT_X] = jctrls[JoystickAxis.LEFT_X] ** 2 * (1 if jctrls[JoystickAxis.LEFT_X] > 0 else -1)
        jctrls[JoystickAxis.LEFT_Y] = jctrls[JoystickAxis.LEFT_Y] ** 2 * (1 if jctrls[JoystickAxis.LEFT_Y] > 0 else -1)
        jctrls[JoystickAxis.RIGHT_X] = jctrls[JoystickAxis.RIGHT_X] ** 2 * (1 if jctrls[JoystickAxis.RIGHT_X] > 0 else -1)

    def _joystickFunc(self, jctrls: dict['JoystickAxis', int]):
        # self.telemetry.success(f"Joystick: {jctrls[JoystickAxis.LEFT_Y]}")
        # self.squareJoystick(jctrls)
        self.drivetrain.robotCentric(jctrls[JoystickAxis.LEFT_X], jctrls[JoystickAxis.LEFT_Y], jctrls[JoystickAxis.RIGHT_X])

        # if jctrls[JoystickAxis.RIGHT_TRIGGER] - jctrls[JoystickAxis.LEFT_TRIGGER] == 1:
        #     if (jctrls[JoystickAxis.RIGHT_TRIGGER] > 0):
        #         self.arm.io.turret.rotateContinuous(StepperDirection.CW)
        #     else:
        #         self.arm.io.turret.rotateContinuous(StepperDirection.CW)
        # else:
        #     self.arm.io.turret.stop()

    def _loadButtons(self):
        self.registerButton(JoystickButton.A, self.pinchers.open)
        self.registerButton(JoystickButton._A, self.pinchers.close)

        self.registerButton(JoystickButton.B, self.arm.stow)

        def _updateTelem():
            self.drivetrain.telemetry.toggleVerbose()
            # self.serial.telemetry.toggleVerbose()
            # self.arm.telemetry.toggleVerbose()
            # self.pinchers.telemetry.toggleVerbose()

        self.registerButton(JoystickButton._BACK, _updateTelem)

        self.registerButton(JoystickButton.LEFTSHOULDER, lambda: self.arm.io.turret.rotateContinuous(StepperDirection.CW))
        self.registerButton(JoystickButton.RIGHTSHOULDER, lambda: self.arm.io.turret.rotateContinuous(StepperDirection.CCW))
        self.registerButton(JoystickButton._LEFTSHOULDER, self.arm.io.turret.stop)
        self.registerButton(JoystickButton._RIGHTSHOULDER, self.arm.io.turret.stop)

        # self.registerButton(JoystickButton.DPADUP, lambda: self.arm.io.arm.rotateContinuous(StepperDirection.CW))
        # self.registerButton(JoystickButton.DPADDOWN, lambda: self.arm.io.arm.rotateContinuous(StepperDirection.CCW))
        self.registerButton(JoystickButton.DPADUP, lambda: self.arm.moveArm(StepperDirection.CW))
        self.registerButton(JoystickButton.DPADDOWN, lambda: self.arm.moveArm(StepperDirection.CCW))
        self.registerButton(JoystickButton._DPADUP, lambda: self.arm.moveArm(StepperDirection.STOP))
        self.registerButton(JoystickButton._DPADDOWN, lambda: self.arm.moveArm(StepperDirection.STOP))

        self.registerButton(JoystickButton.DPADLEFT, lambda: self.arm.io.wrist.rotateContinuous(StepperDirection.CW))
        self.registerButton(JoystickButton.DPADRIGHT, lambda: self.arm.io.wrist.rotateContinuous(StepperDirection.CCW))
        self.registerButton(JoystickButton._DPADLEFT, self.arm.io.wrist.stop)
        self.registerButton(JoystickButton._DPADRIGHT, self.arm.io.wrist.stop)

        # self.registerButton(JoystickButton.X, self.arm.disable)
        # self.registerButton(JoystickButton.Y, self.arm.enable)

        # self.registerButton(JoystickButton.X, self.camera.start)
        # self.registerButton(JoystickButton.Y, self.camera.stop)

        # def highRPM():
        #     self.serial.startMultiCommand()
        #     self.arm.io.turret.setRPM(280)
        #     self.arm.io.arm.setRPM(160)
        #     self.arm.io.wrist.setRPM(160)
        #     self.serial.write()
        
        # def lowRPM():
        #     self.serial.startMultiCommand()
        #     self.arm.io.turret.setRPM(120)
        #     self.arm.io.arm.setRPM(120)
        #     self.arm.io.wrist.setRPM(120)
        #     self.serial.write()

        # self.registerButton(JoystickButton.Y, highRPM)
        # self.registerButton(JoystickButton.X, lowRPM)
