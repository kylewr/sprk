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
    DPADUP = 10
    DPADDOWN = 11
    DPADLEFT = 12
    DPADRIGHT = 13

    _A = 14
    _B = 15
    _X = 16
    _Y = 17
    _LEFTSHOULDER = 18
    _RIGHTSHOULDER = 19
    _BACK = 20
    _START = 21
    _LEFTTHUMB = 22
    _RIGHTTHUMB = 23
    _DPADUP = 24
    _DPADDOWN = 25
    _DPADLEFT = 26
    _DPADRIGHT = 27

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
        DPADUP: "DPADUP",
        DPADDOWN: "DPADDOWN",
        DPADLEFT: "DPADLEFT",
        DPADRIGHT: "DPADRIGHT",
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
        _DPADUP: "-DPADUP",
        _DPADDOWN: "-DPADDOWN",
        _DPADLEFT: "-DPADLEFT",
        _DPADRIGHT: "-DPADRIGHT",
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
        DPADUP: _DPADUP,
        DPADDOWN: _DPADDOWN,
        DPADLEFT: _DPADLEFT,
        DPADRIGHT: _DPADRIGHT,
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

class JoystickAxis(Enum):
    LEFT_X = 0
    LEFT_Y = 1
    RIGHT_X = 2
    RIGHT_Y = 3
    LEFT_TRIGGER = 4
    RIGHT_TRIGGER = 5

    @classmethod
    def convertFromList(cls, axis: list[int]) -> dict['JoystickAxis', float]:
        return {
            JoystickAxis[cls.LEFT_X.name]: int(axis[cls.LEFT_X.value]),
            JoystickAxis[cls.LEFT_Y.name]: int(axis[cls.LEFT_Y.value]),
            JoystickAxis[cls.RIGHT_X.name]: int(axis[cls.RIGHT_X.value]),
            JoystickAxis[cls.RIGHT_Y.name]: int(axis[cls.RIGHT_Y.value]),
            JoystickAxis[cls.LEFT_TRIGGER.name]: int(axis[cls.LEFT_TRIGGER.value]),
            JoystickAxis[cls.RIGHT_TRIGGER.name]: int(axis[cls.RIGHT_TRIGGER.value]),
        }