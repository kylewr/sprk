from robotBase import Subsystem
from robotBase.actuation.GPIOServo import GPIOServo

class Pinchers(Subsystem.Subsystem):
    def __init__(self, servoPin) -> None:
        super().__init__("PINCHERS")
        self.servo = GPIOServo(servoPin, 0, 180)
        self.servo.initGPIO()
    
    def setAngle(self, angle: float) -> None:
        self.servo.setAngle(angle)
    
    def open(self) -> None:
        self.servo.setAngle(0)
        self.telemetry.verbose("Open Pinchers")
    
    def close(self) -> None:
        self.servo.setAngle(180)
        self.telemetry.verbose("Close Pinchers")

    def stop(self) -> None:
        self.servo.stop()
