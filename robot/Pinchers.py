from robotBase import Subsystem
from robotBase.actuation.GPIOServo import GPIOServo

class Pinchers(Subsystem.Subsystem):
    def __init__(self, servoPin) -> None:
        super().__init__("PINCHERS")
        self.servo = GPIOServo(servoPin, 0, 180)
        self.servo.initGPIO()
    
    def setAngle(self, angle: float) -> None:
        self.servo.setAngle(angle)
    
    def stop(self) -> None:
        self.servo.stop()
