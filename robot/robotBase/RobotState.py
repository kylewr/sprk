from enum import Enum

class RobotState(Enum):
    DISABLED = 0
    AUTONOMOUS = 1
    TELEOP = 2