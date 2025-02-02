import serial
import io

# print(serial.tools.list_ports().comports())

piPort = serial.Serial('COM10', 9600, timeout=50)
# ardPort = serial.Serial('COM11', 9600, timeout=50)


while True:
    i = input("ling gong: ") + "\n"
    if i == "":
        break
    print()
    # print(sio.readLine())
    # break

piPort.close()
# ardPort.close()