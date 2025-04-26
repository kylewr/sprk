from enum import Enum

from robotBase.simulation.SimState import SimState

class MotorDirection(Enum):
    FORWARD = 1
    BACKWARD = -1
    STOP = 0

class HBridgeMotor:
    def __init__(self, pins: tuple) -> None:
        self.pin_low = pins[0]
        self.pin_high = pins[1]
        self.pins = pins

        self.speed = 0
        self.inverted = False
    
    """
    Initializes the GPIO pins for the motor.
    Only call this if you are not using a GPIOMap.
    """
    def initGPIO(self) -> None:
        gpio.setup(self.pin_low, gpio.OUT)
        gpio.setup(self.pin_high, gpio.OUT)

    def setInverted(self, inverted: bool) -> None:
        self.inverted = inverted
    
    def set(self, direction: MotorDirection) -> None:
        if direction == MotorDirection.STOP:
            self.speed = 0
            # Set both pins to LOW to stop the motor
            gpio.output(self.pin_low, gpio.LOW)
            gpio.output(self.pin_high, gpio.LOW)
        else:
            self.speed = direction.value * (1 if not self.inverted else -1)
            # Set the appropriate pin based on the direction
            gpio.output(self.pin_low, gpio.HIGH if self.speed < 0 else gpio.LOW)
            gpio.output(self.pin_high, gpio.HIGH if self.speed > 0 else gpio.LOW)
    
    def stop(self) -> None:
        self.set(MotorDirection.STOP)

if SimState.isSimulation():
    from robotBase.simulation.GPIOSim import GPIOSim as gpio
else:
    import RPi.GPIO as gpio