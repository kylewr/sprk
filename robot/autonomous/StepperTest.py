from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from Shark import SHARK

from robotBase.actuation.VirtualStepper import StepperDirection
from robotBase.AutonomousThread import AutonomousThread

class StepperTest(AutonomousThread):
    def run(self):
        self.robot: 'SHARK'

        actions: tuple[function] = (
            lambda: self.robot.arm.telemetry.info("Now testing turret."),
            self.robot.serial.startMultiCommand,
            lambda: self.robot.arm.io.turret.setEnabled(True),
            lambda: self.robot.arm.io.turret.rotateContinuous(StepperDirection.CW),
            self.robot.serial.write,
            self.sleep(1.5),
            lambda: self.robot.arm.io.turret.rotateContinuous(StepperDirection.CCW),
            self.sleep(1.5),

            self.robot.serial.startMultiCommand,
            self.robot.arm.io.turret.stop,
            lambda: self.robot.arm.io.turret.setEnabled(False),
            lambda: self.robot.arm.io.arm.setEnabled(True),
            lambda: self.robot.arm.telemetry.info("Now testing arm."),
            lambda: self.robot.arm.io.arm.rotateContinuous(StepperDirection.CW),
            self.robot.serial.write,
            self.sleep(1.5),
            lambda: self.robot.arm.io.arm.rotateContinuous(StepperDirection.CCW),
            self.sleep(1.5),

            self.robot.serial.startMultiCommand,
            self.robot.arm.io.arm.stop,
            lambda: self.robot.arm.io.arm.setEnabled(False),
            lambda: self.robot.arm.io.wrist.setEnabled(True),
            lambda: self.robot.arm.telemetry.info("Now testing wrist."),
            lambda: self.robot.arm.io.wrist.rotateContinuous(StepperDirection.CW),
            self.robot.serial.write,
            self.sleep(1.5),
            lambda: self.robot.arm.io.wrist.rotateContinuous(StepperDirection.CCW),
            self.sleep(1.5),

            self.robot.serial.startMultiCommand,
            self.robot.arm.io.wrist.stop,
            lambda: self.robot.arm.io.wrist.setEnabled(False),
            self.robot.serial.write,

            lambda: self.robot.arm.telemetry.success("Self test complete."),

            self.endAction
        )

        for action in actions:
            if self.stopped():
                break
            action()