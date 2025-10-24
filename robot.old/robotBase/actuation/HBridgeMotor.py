from enum import Enum

from robotBase.simulation.SimState import SimState

class MotorDirection(Enum):
    FORWARD = 1
    BACKWARD = -1
    STOP = 0

class PWMHBridgeMotor:
    def __init__(self, pins: tuple, pwmFreq: int) -> None:
        self.pin_low = pins[0]
        self.pin_high = pins[1]
        self.pins = pins

        self.pwmFreq = pwmFreq

        self.speed = 0
        self.inverted = False
        self.pwm_low = None
        self.pwm_high = None
    
    def initGPIO(self) -> None:
        gpio.setup(self.pin_low, gpio.OUT)
        gpio.setup(self.pin_high, gpio.OUT)

        self.pwm_low = gpio.PWM(self.pin_low, self.pwmFreq)
        self.pwm_high = gpio.PWM(self.pin_high, self.pwmFreq)

        self.pwm_low.start(0)
        self.pwm_high.start(0)
    
    def setInverted(self, inverted: bool) -> None:
        self.inverted = inverted
    
    def set(self, direction: MotorDirection) -> None:
        self.setWithSpeed(direction.value * 100)

    def setWithSpeed(self, speed: int) -> None:
        # speed is a number -100 to 100
        speed = max(min(speed, 100), -100)
        self.speed = speed
        if abs(speed) < 15: # deadband
            # Stop the PWM signal
            self.stop()
        else:
            invertAppliedSpeed = speed * (1 if not self.inverted else -1)
            self.pwm_low.ChangeDutyCycle(0 if invertAppliedSpeed > 0 else abs(invertAppliedSpeed))
            self.pwm_high.ChangeDutyCycle(0 if invertAppliedSpeed < 0 else abs(invertAppliedSpeed))
    
    def stop(self) -> None:
        self.pwm_low.ChangeDutyCycle(0)
        self.pwm_high.ChangeDutyCycle(0)
    
    def cleanup(self) -> None:
        self.pwm_low.stop()
        self.pwm_high.stop()
        gpio.cleanup(self.pin_low)
        gpio.cleanup(self.pin_high)

class SimpleHBridgeMotor:
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
            self.speed = direction.value
            invertAppliedSpeed = self.speed * (1 if not self.inverted else -1)
            # Set the appropriate pin based on the direction
            gpio.output(self.pin_low, gpio.HIGH if invertAppliedSpeed < 0 else gpio.LOW)
            gpio.output(self.pin_high, gpio.HIGH if invertAppliedSpeed > 0 else gpio.LOW)
    
    def stop(self) -> None:
        self.set(MotorDirection.STOP)
    
    def cleanup(self) -> None:
        gpio.cleanup(self.pin_low)
        gpio.cleanup(self.pin_high)

if SimState.isSimulation():
    from robotBase.simulation.GPIOSim import GPIOSim as gpio
else:
    import RPi.GPIO as gpio