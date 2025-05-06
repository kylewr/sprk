from enum import Enum

class RobotState(Enum):
    DISABLED = 0
    AUTONOMOUS = 1
    TELEOP = 2

class JoystickButton(Enum):
    A = 0
    B = 1
    X = 2
    Y = 3
    LEFTSHOULDER = 4
    RIGHTSHOULDER = 5
    BACK = 6
    START = 7
    LEFTTHUMB = 8
    RIGHTTHUMB = 9

    _A = 10
    _B = 11
    _X = 12
    _Y = 13
    _LEFTSHOULDER = 14
    _RIGHTSHOULDER = 15
    _BACK = 16
    _START = 17
    _LEFTTHUMB = 18
    _RIGHTTHUMB = 19

    _NAMES = {
        A: "A",
        B: "B",
        X: "X",
        Y: "Y",
        LEFTSHOULDER: "LEFTSHOLDER",
        RIGHTSHOULDER: "RIGHTSHOULDER",
        BACK: "BACK",
        START: "START",
        LEFTTHUMB: "LEFTTHUMB",
        RIGHTTHUMB: "RIGHTHUMB",
        _A: "-A",
        _B: "-B",
        _X: "-X",
        _Y: "-Y",
        _LEFTSHOULDER: "-LEFTSHOLDER",
        _RIGHTSHOULDER: "-RIGHTSHOULDER",
        _BACK: "-BACK",
        _START: "-START",
        _LEFTTHUMB: "-LEFTTHUMB",
        _RIGHTTHUMB: "-RIGHTTHUMB",
    }

    _RELEASE_MAP = {
        A: _A,
        B: _B,
        X: _X,
        Y: _Y,
        LEFTSHOULDER: _LEFTSHOULDER,
        RIGHTSHOULDER: _RIGHTSHOULDER,
        BACK: _BACK,
        START: _START,
        LEFTTHUMB: _LEFTTHUMB,
        RIGHTTHUMB: _RIGHTTHUMB,
    }

    @classmethod
    def getReleasedValue(cls, button) -> 'JoystickButton':
        return JoystickButton(cls._RELEASE_MAP.value[button]) if button in cls._RELEASE_MAP.value else button

    @classmethod
    def getStr(cls) -> str:
        return cls._NAMES.value[cls.value]

    @classmethod
    def getAll(cls) -> list[str]:
        return cls._NAMES.value.values()
