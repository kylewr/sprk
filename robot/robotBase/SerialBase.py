class Serial:
    def __init__(self, port, baudrate):
        self.port = port
        self.baudrate = baudrate
        self.is_open = False

    def open(self):
        if not self.is_open:
            print(f"Opening serial port {self.port} at {self.baudrate} baud.")
            self.is_open = True
        else:
            print("Serial port is already open.")

    def close(self):
        if self.is_open:
            print(f"Closing serial port {self.port}.")
            self.is_open = False
        else:
            print("Serial port is already closed.")