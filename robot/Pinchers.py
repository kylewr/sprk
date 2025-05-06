from robotBase import Subsystem
from robotBase.actuation.GPIOServo import GPIOServo

from Constants import Constants

class Pinchers(Subsystem.Subsystem):
    def __init__(self, servoPin) -> None:
        super().__init__("PINCHERS")
        self.servo = GPIOServo(servoPin, Constants.ActuationConstants.PINCHER_MIN, Constants.ActuationConstants.PINCHER_MAX)
        self.servo.initGPIO()
    
    def setAngle(self, angle: float) -> None:
        self.servo.setAngle(angle)
    
    def open(self) -> None:
        self.servo.setAngle(Constants.ActuationConstants.PINCHER_OPEN)
        self.telemetry.verbose("Open Pinchers")
    
    def close(self) -> None:
        self.servo.setAngle(Constants.ActuationConstants.PINCHER_CLOSE)
        self.telemetry.verbose("Close Pinchers")

    def stop(self) -> None:
        self.servo.stop()
