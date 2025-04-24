from Telemetry import Telemetry, TelemetrySubsystem

class Subsystem:
    def __init__(self, name: str) -> None:
        self.telemetry = TelemetrySubsystem(name)
    
    def addTelemetry(self, telem: Telemetry) -> None:
        self.telemetry.pass_telem(telem)
        self.telemetry.info("Telemetry added.")
