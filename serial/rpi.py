from serial import Serial
from threading import Thread
from time import sleep

port = Serial('COM10', timeout=50)

def flush():
    while True:
        port.reset_input_buffer()
        port.reset_output_buffer()
        sleep(60)

def listen():
    while True:
        inp = port.read(1)
        decoded = inp.decode()
        print(repr(inp))
        # port.write(inp)

listener = Thread(None, listen, daemon=True)
flusher = Thread(None, flush, daemon=True)

listener.start()
flusher.start()

while True:
    res = input("ext: ")
    if res == "y":
        break

port.close()