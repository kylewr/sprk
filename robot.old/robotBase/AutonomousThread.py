import threading
from time import sleep as timesleep

from robotBase.RobotBase import RobotBase

class AutonomousThread(threading.Thread):
    def __init__(self, *args, **kwargs):
        super(AutonomousThread, self).__init__(*args, **kwargs)
        super().__setattr__('daemon', True)
        self._stop_event = threading.Event()
        self.robot: RobotBase = None
        self.endAction: function = lambda: None

    def passRobot(self, robot: RobotBase):
        self.robot = robot
        self.robot.useHeartbeatCheck = False
    
    def withEndAction(self, action):
        self.endAction: function = action

    def sleep(self, seconds: float):
        if self.stopped():
            return
        return lambda: timesleep(seconds)

    def stop(self):
        self._stop_event.set()
    
    def stopped(self):
        return self._stop_event.is_set()

