from robotBase.simulation.SimState import SimState

from threading import Thread
from time import sleep

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
        # self.angle = max(min(angle, self.max_angle), self.min_angle)
        angleTransform = angle

        duty_cycle = (angleTransform / 18) + 2

        def setServo():
            self.pwm.ChangeDutyCycle(duty_cycle)
            sleep(0.75)
            self.pwm.ChangeDutyCycle(0)
        
        Thread(target=setServo,daemon=True).start()

    
    def stop(self) -> None:
        self.pwm.ChangeDutyCycle(0)

    def cleanup(self):
        self.pwm.stop()
        gpio.cleanup(self.pinNum)
    
if SimState.isSimulation():
    from robotBase.simulation.GPIOSim import GPIOSim as gpio
else:
    import RPi.GPIO as gpio
