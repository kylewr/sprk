class Constants:

    SIM_SERIAL_PORT = "COM6"
    SERIAL_PORT = "/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_134484749393518008D8-if00"
    BAUD_RATE = 115200

    class GPIOMap:
        FL = (3, 5)
        FR = (11, 13)
        BL = (29, 31) #(38, 36)
        BR = (21, 23) #(37, 35)

        SERVO = 8

    class SerialMap:
        TURRET = 0
        ARM = 1
        WRIST = 2
    
    class ActuationConstants:
        PINCHER_OPEN = 0
        PINCHER_CLOSE = 180
        PINCHER_MIN = 0
        PINCHER_MAX = 180

        POSITION_STOW = (0, 0, 0)
