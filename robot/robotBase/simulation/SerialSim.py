class SerialSim:
    def __init__(self, port, baudrate):
        self.port = port
        self.baudrate = baudrate

    def open(self):
        print(f"Opening simulation serial port {self.port} at {self.baudrate} baud.")

    def close(self):
        print(f"Closing simulation serial port {self.port}.")
    
    def write(self, data):
        print(f"[SerialSim] {self.port}: {data}")
    
