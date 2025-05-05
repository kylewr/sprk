from time import sleep
from threading import Thread

from robotBase.simulation.SimState import SimState
from robotBase.Telemetry import Telemetry, TelemetrySubsystem

class SerialBase:
    def __init__(self, port, baudrate):
        self.port = port
        self.baudrate = baudrate
        self.telemetry = TelemetrySubsystem(f"{port}")

        self._flushThread = Thread(target=self._flush, daemon=True)
        self._listenThread = Thread(target=self._listen, daemon=True)

        self.isInMultiCommand = False
        self.multiCommand = ""

    def addTelemetry(self, telem: Telemetry) -> None:
        self.telemetry.pass_telem(telem)
        self.telemetry.info("Telemetry added.")

    def _flush(self):
        while True:
            self.port.reset_input_buffer()
            self.port.reset_output_buffer()
            sleep(60)
    
    def _listen(self):
        msg = ''
        while True:
            inp = self.port.read(1)
            decoded = inp.decode("utf-8")
            
            if decoded == '' or decoded == '\n':
                continue

            if decoded == '\r': # new line (carriage return)
                self.telemetry.verbose(f"Received: \"{msg}\"") 
                msg = ''
            else:
                msg += decoded

    def open(self):
        self.telemetry.info(f"Attempting to open serial port {self.port} at {self.baudrate} baud.")
        self.port = Serial(self.port, self.baudrate)
        self._flushThread.start()
        self._listenThread.start()

    def close(self):
        self.telemetry.info(f"Closing serial port {self.port}.")
        self.port.close()
        self._flushThread.join()
        self._listenThread.join()
        self._flushThread = Thread(target=self._flush, daemon=True)
        self._listenThread = Thread(target=self._listen, daemon=True)
    
    def write(self, data):
        if (self.isInMultiCommand):
            if data == '\n':
                data = f";{self.multiCommand[:-1]}"
                self.multiCommand = ""
                self.isInMultiCommand = False
            else:
                self.multiCommand += f"{data}."
                return
        self.port.write(data.encode())
        self.telemetry.verbose(f"Sent: {data}")
    
    def startMultiCommand(self):
        self.isInMultiCommand = True
        self.multiCommand = ""

if SimState.isSimulation() and SimState.SIM_SERIAL:
    from robotBase.simulation.SerialSim import SerialSim as Serial
else:
    from serial import Serial