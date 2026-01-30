from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from SPRK import SPRK

from robotBase.AutonomousThread import AutonomousThread

class Demo(AutonomousThread):
    def run(self):
        self.robot: 'SPRK'  # define typing for SHARK

        actions = (
            self.robot.drivetrain.stop,
            self.sleep(1),

            lambda: self.robot.drivetrain.robotCentric(0, 100, 0),
            lambda: self.robot.drivetrain.telemetry.info("forward"),
            self.sleep(1),
            self.robot.drivetrain.stop,
            self.sleep(0.5),
            lambda: self.robot.drivetrain.robotCentric(0, -100, 0),
            lambda: self.robot.drivetrain.telemetry.info("backward"),
            self.sleep(2),
            self.robot.drivetrain.stop,
            self.sleep(0.5),
            lambda: self.robot.drivetrain.robotCentric(0, 100, 0),
            lambda: self.robot.drivetrain.telemetry.info("forward"),
            self.sleep(1),

            self.robot.drivetrain.stop,
            self.sleep(1),

            lambda: self.robot.drivetrain.robotCentric(-100, 0, 0),
            lambda: self.robot.drivetrain.telemetry.info("left"),
            self.sleep(1),
            self.robot.drivetrain.stop,
            self.sleep(0.5),
            lambda: self.robot.drivetrain.robotCentric(100, 0, 0),
            lambda: self.robot.drivetrain.telemetry.info("right"),
            self.sleep(2),
            self.robot.drivetrain.stop,
            self.sleep(0.5),
            lambda: self.robot.drivetrain.robotCentric(-100, 0, 0),
            lambda: self.robot.drivetrain.telemetry.info("left"),
            self.sleep(1),

            self.robot.drivetrain.stop,
            self.sleep(1),

            lambda: self.robot.drivetrain.robotCentric(0, 0, 100),
            lambda: self.robot.drivetrain.telemetry.info("rotate right"),
            self.sleep(2),
            self.robot.drivetrain.stop,
            self.sleep(0.5),
            lambda: self.robot.drivetrain.robotCentric(0, 0, -100),
            lambda: self.robot.drivetrain.telemetry.info("rotate left"),
            self.sleep(2)
        )

        # the one liner :)
        # runs every action until self.stopped() is true
        while not self.stopped():
            [action() if not self.stopped() else None for action in actions]