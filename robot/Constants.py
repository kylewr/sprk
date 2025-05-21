class Constants:

    SIM_SERIAL_PORT = "COM6"
    SERIAL_PORT = "/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_134484749393518008D8-if00"
    BAUD_RATE = 115200

    class CameraConstants:
        EXPOSURE = 50
        FPS = 20
        RES = "1280x1024"

    class GPIOMap:
        PWM_FREQ = 4000
        FL = (3, 5)
        FR = (11, 13)
        BL = (29, 31)
        BR = (21, 23)

        FL_INVERT = False
        FR_INVERT = False
        BL_INVERT = True
        BR_INVERT = True

        SERVO = 8

    class SerialMap:
        TURRET = 0
        ARM = 1
        WRIST = 2
    
    class ActuationConstants:
        PINCHER_OPEN = 50
        PINCHER_CLOSE = 0
        PINCHER_MIN = 0
        PINCHER_MAX = 270

        POSITION_STOW = (0, 0, 0)
