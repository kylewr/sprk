from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from Shark import SHARK

from robotBase import AutonomousThread
from robotBase.actuation.HBridgeMotor import MotorDirection

class DrivetrainTest(AutonomousThread.AutonomousThread):
    def getModuleSpeedAction(self, module: int):
        return (
            lambda: self.robot.drivetrain.telemetry.success(f"Now testing module {module} FORWARD"),
            lambda: self.robot.drivetrain.io.modules[module].set(MotorDirection.FORWARD),
            self.sleep(.75),
            lambda: self.robot.drivetrain.io.modules[module].stop(),
            self.sleep(.75),
            lambda: self.robot.drivetrain.telemetry.info(f"Now testing module {module} BACKWARD"),
            lambda: self.robot.drivetrain.io.modules[module].set(MotorDirection.BACKWARD),
            self.sleep(.75),
            lambda: self.robot.drivetrain.io.modules[module].stop(),
            self.sleep(1.5),
        )

    def run(self):
        self.robot: 'SHARK' # define typing for SHARK

        actions = [
            *self.getModuleSpeedAction(0),
            *self.getModuleSpeedAction(1),
            *self.getModuleSpeedAction(2),
            *self.getModuleSpeedAction(3),
            lambda: self.robot.drivetrain.io.stop(),
            lambda: self.robot.drivetrain.telemetry.warn("Drivetrain test complete"),
            self.sleep(0.2),
            self.endAction,
        ]
        for action in actions:
            if self.stopped():
                break
            action()