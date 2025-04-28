from robotBase import AutonomousThread
from robotBase.actuation.HBridgeMotor import MotorDirection

from time import sleep

class DrivetrainTest(AutonomousThread.AutonomousThread):
    def getModuleSpeedAction(self, module: int):
        return (
            lambda: self.robot.drivetrain.telemetry.warn(f"Now testing module {module} FORWARD"),
            lambda: self.robot.drivetrain.ioMap.modules[module].set(MotorDirection.FORWARD),
            lambda: sleep(1.5),
            lambda: self.robot.drivetrain.ioMap.modules[module].stop(),
            lambda: sleep(1),
            lambda: self.robot.drivetrain.telemetry.info(f"Now testing module {module} BACKWARD"),
            lambda: self.robot.drivetrain.ioMap.modules[module].set(MotorDirection.BACKWARD),
            lambda: sleep(1.5),
            lambda: self.robot.drivetrain.ioMap.modules[module].stop(),
            lambda: sleep(2.5),
        )

    def run(self):
        actions = [
            *self.getModuleSpeedAction(0),
            *self.getModuleSpeedAction(1),
            *self.getModuleSpeedAction(2),
            *self.getModuleSpeedAction(3),
            lambda: self.robot.drivetrain.ioMap.stop(),
            lambda: self.robot.drivetrain.telemetry.warn("Drivetrain test complete"),
            lambda: sleep(0.2),
            self.endAction,
        ]
        for action in actions:
            if self.stopped():
                break
            action()