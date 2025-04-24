import socket

from robotBase.RobotState import RobotState
from Robot import Robot

def handleTeleop(packet: str, robot: Robot) -> None:
    match packet.split(",")[0]:
        case "s":
            robot.drivetrain.stop()
        case "d":
            dctrls = packet.split(",")
            try:
                robot.drivetrain.robotCentric(int(dctrls[1]), int(dctrls[2]), int(dctrls[3]))
            except ValueError:
                robot.telemetry.info(f"Invalid teleop drive command: {packet}")
        case _:
            robot.telemetry.info(f"Recieved an unknown teleop input: {packet}")

def main():
    mainSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    mainSocket.bind(('localhost', 8008))

    mainSocket.listen(1)

    robot = Robot()
    try:
        while True:
            bigBreak = False
            robot.disabledInit()
            print("\033[93mAwaiting connection...\033[0m")
            conn, addr = mainSocket.accept()
            print(f'\033[92mConnected to controller. {addr}\033[0m')

            while True:
                data = conn.recv(256)
                if not data:
                    break
                message = data.decode('utf-8').lower()

                if message.startswith("te-"): # teleop packet
                    if robot.state == RobotState.TELEOP:
                        handleTeleop(message[3:], robot)

                if message == "exit":
                    bigBreak = True
                    break
                elif message == "auton":
                    robot.autonomousInit()
                elif message.startswith("tele"):
                    robot.teleopInit()
                elif message.startswith("dis"):
                    robot.disabledInit()

            robot.disabledInit()
            print(f"\033[91mSocket lost connection! {addr}\033[0m")
            if (bigBreak):
                break
    except KeyboardInterrupt:
        robot.disabledInit()

if __name__ == '__main__':
    main()