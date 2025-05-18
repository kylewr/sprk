from robotBase.simulation.SimState import SimState

class GPIOServo:
    def __init__(self, pin, min_angle=0, max_angle=180):
        self.pinNum = pin
        self.min_angle = min_angle
        self.max_angle = max_angle
        self.angle = 0

    def initGPIO(self):
        gpio.setup(self.pinNum, gpio.OUT)
        self.pwm = gpio.PWM(self.pinNum, 50)
        self.pwm.start(0)
        
    def setAngle(self, angle: float) -> None:
        self.angle = max(min(angle, self.max_angle), self.min_angle)
        angleTransform = self.angle

        duty_cycle = (angleTransform / 18) + 2
        self.pwm.ChangeDutyCycle(duty_cycle)
    
    def stop(self) -> None:
        self.pwm.ChangeDutyCycle(0)

    def cleanup(self):
        self.pwm.stop()
        gpio.cleanup(self.pinNum)
    
if SimState.isSimulation():
    from robotBase.simulation.GPIOSim import GPIOSim as gpio
else:
    import RPi.GPIO as gpio
