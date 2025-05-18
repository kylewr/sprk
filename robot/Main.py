import socket
from time import sleep
from threading import Thread

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
            simFlag = ",sim" if SimState.isSimulation() else ""
            robot.telemetry.sendDS(f"[ROBOTINFO] \nBuild Time: {SimState.getBuildTime()}\nSimluation: {SimState.isSimulation()}[SIG][AUTONS]{robot.getAutonsAsCSV()}[SIG][FLAGS]{simFlag},camera,\n")

            while True:
                try:
                    data = conn.recv(256)
                except socket.error as e:
                    print(f"\033[91mSocket error: {e}\033[0m")
                    break
                if not data:
                    break
                message = data.decode('utf-8')

                robot.timeSinceLastPacket = 0
                if message.startswith("te-"): # teleop packet
                    robot.handleTeleop(message[3:])
                elif message.startswith("init"):
                    robot.telemetry.success("Recieved Driver Station info: " + message[5:])
                elif message.startswith("se-auto,"):
                    robot.selectAuton(message[8:])
                elif message.startswith("cam-"):
                    if message[4:].startswith("start"):
                        robot.camera.start()
                    elif message[4:].startswith("stop"):
                        robot.camera.stop()

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
        robot.cleanup()
    except KeyboardInterrupt:
        robot.emergencyStop()
        robot.cleanup()

if __name__ == '__main__':
    global lastHeartbeat, heartbeatThread
    main()