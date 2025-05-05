from robotBase.Telemetry import Telemetry, TelemetrySubsystem

class GPIOSim:
    LOW = 0
    HIGH = 1

    OUT = 0
    IN = 1

    BOARD = 10
    BCM = 11

    telemetry = TelemetrySubsystem("GPIO-SIM")

    @staticmethod
    def addTelemetry(telemetry: Telemetry) -> None:
        GPIOSim.telemetry.pass_telem(telemetry)

    @staticmethod
    def setmode(mode) -> None:
        GPIOSim.telemetry.info(f"`setmode` called with mode: {mode}")

    @staticmethod
    def setup(pin, mode) -> None:
        GPIOSim.telemetry.info(f"`setup` called with pin: {pin} and mode: {mode}")
    
    @staticmethod
    def output(pin, state) -> None:
        GPIOSim.telemetry.verbose(f"`output` called with pin: {pin} and state: {state}")
    
    class PWM:
        def __init__(self, pin, frequency) -> None:
            self.pin = pin
            self.frequency = frequency
            self.duty_cycle = 0.0
            GPIOSim.telemetry.info(f"`PWM` initialized with pin: {self.pin} and frequency: {frequency}")
        
        def start(self, duty_cycle) -> None:
            self.duty_cycle = duty_cycle
            GPIOSim.telemetry.verbose(f"Pin {self.pin}: `start` called with duty cycle: {duty_cycle}")
            
        def ChangeDutyCycle(self, duty_cycle) -> None:
            self.duty_cycle = duty_cycle
            GPIOSim.telemetry.verbose(f"Pin {self.pin}: `ChangeDutyCycle` called with duty cycle: {duty_cycle}")