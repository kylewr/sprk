from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from Shark import SHARK

from robotBase.AutonomousThread import AutonomousThread

class Demo(AutonomousThread):
    def run(self):
        robot: SHARK = self.robot

        actions = [
            robot.drivetrain.stop,
            self.sleep(1),

            lambda: robot.drivetrain.robotCentric(0, 1, 0),
            lambda: robot.drivetrain.telemetry.info("forward"),
            self.sleep(1),
            robot.drivetrain.stop,
            self.sleep(0.5),
            lambda: robot.drivetrain.robotCentric(0, -1, 0),
            lambda: robot.drivetrain.telemetry.info("backward"),
            self.sleep(2),
            robot.drivetrain.stop,
            self.sleep(0.5),
            lambda: robot.drivetrain.robotCentric(0, 1, 0),
            lambda: robot.drivetrain.telemetry.info("forward"),
            self.sleep(1),

            robot.drivetrain.stop,
            self.sleep(1),

            lambda: robot.drivetrain.robotCentric(-1, 0, 0),
            lambda: robot.drivetrain.telemetry.info("left"),
            self.sleep(1),
            robot.drivetrain.stop,
            self.sleep(0.5),
            lambda: robot.drivetrain.robotCentric(1, 0, 0),
            lambda: robot.drivetrain.telemetry.info("right"),
            self.sleep(2),
            robot.drivetrain.stop,
            self.sleep(0.5),
            lambda: robot.drivetrain.robotCentric(-1, 0, 0),
            lambda: robot.drivetrain.telemetry.info("left"),
            self.sleep(1),

            robot.drivetrain.stop,
            self.sleep(1),

            lambda: robot.drivetrain.robotCentric(0, 0, 1),
            lambda: robot.drivetrain.telemetry.info("rotate right"),
            self.sleep(2),
            robot.drivetrain.stop,
            self.sleep(0.5),
            lambda: robot.drivetrain.robotCentric(0, 0, -1),
            lambda: robot.drivetrain.telemetry.info("rotate left"),
            self.sleep(2)
        ]
        while not self.stopped():
            for action in actions:
                if not self.stopped():
                    action()
                else:
                    break