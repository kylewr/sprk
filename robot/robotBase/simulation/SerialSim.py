from robotBase.Telemetry import Telemetry, TelemetrySubsystem

class SerialSim:
    def __init__(self, port, baudrate):
        self.port = port
        self.baudrate = baudrate

        self.telemetry = TelemetrySubsystem(f"{port}-SIM")

        self.isInMultiCommand = False
        self.multiCommand = ""

    def addTelemetry(self, telem: Telemetry) -> None:
        self.telemetry.pass_telem(telem)
        self.telemetry.info("Telemetry added.")

    def open(self):
        self.telemetry.sim(f"Opening simulation serial port {self.port} at {self.baudrate} baud.")

    def close(self):
        self.telemetry.sim(f"Closing simulation serial port {self.port}.")
    
    def write(self, data):
        if (self.isInMultiCommand):
            self.multiCommand += data
            if data == '\n':
                data = self.multiCommand
                self.multiCommand = ""
                self.isInMultiCommand = False
        self.telemetry.sim(f"Write: {data}")
    
    def startMultiCommand(self):
        self.isInMultiCommand = True
        self.multiCommand = ""
