import socket
from time import sleep

from robotBase.RobotState import RobotState
from robotBase.simulation.SimState import SimState
from Shark import SHARK

def main():
    mainSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    hostname = 'QuackStation' if SimState.isSimulation() else 'shark.local'
    mainSocket.bind((hostname, 8008))

    mainSocket.listen(5)

    robot = SHARK()
    try:
        while True:
            bigBreak = False
            robot.disabledInit()
            print(f'\033[90mAwaiting connection on {mainSocket.getsockname()}...\033[0m')
            conn, addr = mainSocket.accept()

            def sendDs(msg):
                try:
                    conn.sendall(msg.encode('utf-8'))
                except socket.error as e:
                    pass

            robot.telemetry.setDSMessageSupplier(sendDs)
            print(f'\033[92mConnected to controller. {addr}\033[0m')

            robot.telemetry.sendDS("[STATE] DISABLE")
            sleep(0.5)

            robot.telemetry.sendDS(f"[ROBOTINFO] \nRobot Author: Kyle Rush\nBuild Time: {SimState.getBuildTime()}\nRobot Version: v2025.04")

            while True:
                try:
                    data = conn.recv(256)
                except socket.error as e:
                    print(f"\033[91mSocket error: {e}\033[0m")
                    break
                if not data:
                    break
                message = data.decode('utf-8').lower()
                if message.startswith("te-"): # teleop packet
                    robot.handleTeleop(message[3:])

                if message.startswith("exit"):
                    bigBreak = True
                    break
                elif message.startswith("auto"):
                    robot.autonomousInit()
                elif message.startswith("tele"):
                    robot.teleopInit()
                elif message.startswith("dis"):
                    robot.disabledInit()

            if (robot.state != RobotState.DISABLED):
                robot.emergencyStop()
            print(f"\033[90mSocket lost connection! {addr}\033[0m")
            if (bigBreak):
                break
    except KeyboardInterrupt:
        robot.emergencyStop()

if __name__ == '__main__':
    main()