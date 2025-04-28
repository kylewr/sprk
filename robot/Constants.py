class Constants:

    SERIAL_PORT = "COM4"
    BAUD_RATE = 115200

    class GPIOMap:
        FL = (3, 5)
        FR = (11, 13)
        BL = (38, 36)
        BR = (37, 35)

    class SerialMap:
        TURRET = 0
        ARM = 1
        WRIST = 2