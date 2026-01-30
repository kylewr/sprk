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
        self.telemetry.verbose(f"Opening serial port {self.port} at {self.baudrate} baud.")

    def close(self):
        self.telemetry.warn(f"Closing serial port {self.port}.")
    
    def write(self, data = "\n"):
        if (self.isInMultiCommand):
            if data == '\n':
                data = f";{self.multiCommand[:-1]}"
                self.multiCommand = ""
                self.isInMultiCommand = False
            else:
                self.multiCommand += f"{data}."
                return
        self.telemetry.verbose(f"Sent: {data}")
    
    def startMultiCommand(self):
        self.isInMultiCommand = True
        self.multiCommand = ""
