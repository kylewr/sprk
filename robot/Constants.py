class Constants:

    SERIAL_PORT = "COM4"
    BAUD_RATE = 115200

    class GPIOMap:
        FL   = (3, 5)
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
