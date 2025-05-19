from typing import TYPE_CHECKING
import time
import math

if TYPE_CHECKING:
    from Shark import SHARK

from robotBase.AutonomousThread import AutonomousThread

class CoolDrive(AutonomousThread):
    def run(self):
        self.robot: 'SHARK'

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
            steps = 40
            for i in range(steps + 1):
                if self.stopped():
                    break
                t = i / steps * (math.pi * 2 * cycles)
                x = int(math.sin(t) * amplitude)
                y = forward_speed
                self.robot.drivetrain.robotCentric(x, y, 0)
                time.sleep(duration / steps)
            self.robot.drivetrain.telemetry.info("end zigzag")

        def orbit_move(radius_power=50, spin_power=30, duration=3):
            """Drive forward while rotating to make a sweeping arc."""
            self.robot.drivetrain.telemetry.info("start orbit move")
            steps = 30
            for i in range(steps):
                if self.stopped():
                    break
                self.robot.drivetrain.robotCentric(radius_power, radius_power, spin_power)
                time.sleep(duration / steps)
            self.robot.drivetrain.telemetry.info("end orbit move")

        actions = (
            self.robot.drivetrain.stop,
            self.sleep(0.25),

            # Smooth ramp forward and reverse
            lambda: ramp_velocity(0, 100, 0, duration=2, label="forward ramp"),
            self.sleep(0.5),
            lambda: ramp_velocity(0, -100, 0, duration=2, label="reverse ramp"),
            self.sleep(0.5),

            # Spin in place
            lambda: ramp_velocity(0, 0, 100, duration=2, label="spin right"),
            self.sleep(0.5),
            lambda: ramp_velocity(0, 0, -100, duration=2, label="spin left"),
            self.sleep(0.5),

            self.robot.drivetrain.stop,
            self.sleep(0.5),

            # Zigzag without pause
            lambda: zigzag_strafe(amplitude=100, forward_speed=40, cycles=2, duration=2),

            # Orbiting move — curved drift
            lambda: orbit_move(radius_power=60, spin_power=30, duration=2),

            # orbiting move - drift opposite
            lambda: orbit_move(radius_power=-60, spin_power=-30, duration=2),

            self.robot.drivetrain.stop,
            self.sleep(0.5)
        )

        while not self.stopped():
            [action() if not self.stopped() else None for action in actions]
