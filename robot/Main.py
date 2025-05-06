import socket
from time import sleep

from robotBase.RobotEnums import RobotState
from robotBase.simulation.SimState import SimState
from Shark import SHARK

def main():
    mainSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    mainSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    hostname = 'QuackStation' if SimState.isSimulation() else '10.8.62.2'
    mainSocket.bind((hostname, 8008))

    mainSocket.listen(5)

    robot = SHARK()
    try:
        robotAlive = True
        while robotAlive:
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
            sleep(0.2)

            robot.telemetry.sendDS(f"[ROBOTINFO] \nBuild Time: {SimState.getBuildTime()}")

            while True:
                try:
                    data = conn.recv(256)
                except socket.error as e:
                    print(f"\033[91mSocket error: {e}\033[0m")
                    break
                if not data:
                    break
                message = data.decode('utf-8')
                if message.startswith("te-"): # teleop packet
                    robot.handleTeleop(message[3:])

                if message.startswith("exit"):
                    robotAlive = False
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
        conn.close()
    except KeyboardInterrupt:
        robot.emergencyStop()

if __name__ == '__main__':
    main()