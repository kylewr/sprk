import RPi.GPIO as gpio
from time import sleep

import drivetrain

mdrive = drivetrain.MecanumDrive(drivetrain.MecanumIOMap(3, 5, 11, 13, 37, 35, 38, 36))

try:

    while True:
        d = input("send: ")
        if (d.startswith("d")):
            indCommands = list(d)
            mdrive.drive(2 - int(d[2]), 2 - int(d[3]), 2 - int(d[4]))
            print("drive")
            sleep(int(d[1]))
            mdrive.stop()
            print("stop")
except KeyboardInterrupt:

    mdrive.stop()
    print("stop")