class Constants:

    SIM_SERIAL_PORT = "COM5"
    SERIAL_PORT = "/dev/serial/by-id/usb-Arduino__www.arduino.cc__0042_34331323635351010260-if00"
    BAUD_RATE = 115200

    class GPIOMap:
        FL = (3, 5)
        FR = (11, 13)
        BL = (37, 35)
        BR = (38, 36)

        SERVO = 7

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
