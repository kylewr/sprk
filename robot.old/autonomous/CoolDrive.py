from typing import TYPE_CHECKING
import time
import math

if TYPE_CHECKING:
    from SPRK import SPRK

from robotBase.AutonomousThread import AutonomousThread

class CoolDrive(AutonomousThread):
    def run(self):
        self.robot: 'SPRK'

        def ramp_velocity(x_target, y_target, r_target, duration, steps=20, label="ramping"):
            """Smooth ramp to a velocity over time using PWM."""
            self.robot.drivetrain.telemetry.info(f"start {label}")
            for i in range(steps + 1):
                if self.stopped():
                    break
                t = i / steps
                x = int(x_target * t)
                y = int(y_target * t)
                r = int(r_target * t)
                self.robot.drivetrain.robotCentric(x, y, r)
                time.sleep(duration / steps)
            self.robot.drivetrain.telemetry.info(f"end {label}")

        def zigzag_strafe(amplitude=100, forward_speed=50, cycles=2, duration=2):
            """Zig-zag left/right while moving forward."""
            self.robot.drivetrain.telemetry.info("start zigzag")
            steps = 80
            for i in range(steps + 1):
                if self.stopped():
                    break
                t = i / steps * (math.pi * 2 * cycles)
                x = int(math.sin(t) * amplitude)
                y = forward_speed
                self.robot.drivetrain.robotCentric(x, y, 0)
                time.sleep(duration / steps)
            self.robot.drivetrain.telemetry.info("end zigzag")

        actions = (
            self.robot.drivetrain.stop,
            self.sleep(0.25),

            # Smooth ramp forward and reverse
            lambda: ramp_velocity(0, 100, 0, duration=2, label="forward ramp"),
            lambda: ramp_velocity(0, -100, 0, duration=2, label="reverse ramp"),
            self.sleep(0.25),

            # Smooth ramp left and right
            lambda: ramp_velocity(100, 0, 0, duration=2, label="right ramp"),
            lambda: ramp_velocity(-100, 0, 0, duration=2, label="left ramp"),
            self.sleep(0.25),

            self.robot.drivetrain.stop,

            # Zigzag without pause
            lambda: zigzag_strafe(amplitude=100, forward_speed=70, cycles=4, duration=4),
            lambda: zigzag_strafe(100, -70, 4, 4),

            self.robot.drivetrain.stop,
            self.sleep(0.5),

            self.endAction
        )

        [action() if not self.stopped() else None for action in actions]
