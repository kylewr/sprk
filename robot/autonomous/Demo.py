from robotBase.AutonomousThread import AutonomousThread
from time import sleep

class Demo(AutonomousThread):
    def run(self):
        actions = [
            lambda: self.robot.drivetrain.stop(),
            lambda: sleep(1),

            lambda: self.robot.drivetrain.robotCentric(0, 1, 0),
            lambda: self.robot.drivetrain.telemetry.info("forward"),
            lambda: sleep(1),
            lambda: self.robot.drivetrain.stop(),
            lambda: sleep(0.5),
            lambda: self.robot.drivetrain.robotCentric(0, -1, 0),
            lambda: self.robot.drivetrain.telemetry.info("backward"),
            lambda: sleep(2),
            lambda: self.robot.drivetrain.stop(),
            lambda: sleep(0.5),
            lambda: self.robot.drivetrain.robotCentric(0, 1, 0),
            lambda: self.robot.drivetrain.telemetry.info("forward"),
            lambda: sleep(1),

            lambda: self.robot.drivetrain.stop(),
            lambda: sleep(1),

            lambda: self.robot.drivetrain.robotCentric(-1, 0, 0),
            lambda: self.robot.drivetrain.telemetry.info("left"),
            lambda: sleep(1),
            lambda: self.robot.drivetrain.stop(),
            lambda: sleep(0.5),
            lambda: self.robot.drivetrain.robotCentric(1, 0, 0),
            lambda: self.robot.drivetrain.telemetry.info("right"),
            lambda: sleep(2),
            lambda: self.robot.drivetrain.stop(),
            lambda: sleep(0.5),
            lambda: self.robot.drivetrain.robotCentric(-1, 0, 0),
            lambda: self.robot.drivetrain.telemetry.info("left"),
            lambda: sleep(1),

            lambda: self.robot.drivetrain.stop(),
            lambda: sleep(1),

            lambda: self.robot.drivetrain.robotCentric(0, 0, 1),
            lambda: self.robot.drivetrain.telemetry.info("rotate right"),
            lambda: sleep(2),
            lambda: self.robot.drivetrain.stop(),
            lambda: sleep(0.5),
            lambda: self.robot.drivetrain.robotCentric(0, 0, -1),
            lambda: self.robot.drivetrain.telemetry.info("rotate left"),
            lambda: sleep(2)
        ]
        for action in actions:
            if not self.stopped():
                action()
            else:
                break