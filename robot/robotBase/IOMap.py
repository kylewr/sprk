from simulation.SimState import SimState

class GPIOMap:
    def __init__(self) -> None:
        self.pins = []
        gpio.setmode(gpio.BOARD) # Set the GPIO mode to BOARD
    
    def addPin(self, pin: int) -> None:
        self.pins.append(pin)
    
    def initOut(self) -> None:
        [gpio.setup(pin, gpio.OUT) for pin in self.pins]

if SimState.isSimulation():
    from simulation.GPIOSim import GPIOSim as gpio
else:
    import RPi.GPIO as gpio # type: ignore