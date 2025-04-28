import threading

class AutonomousThread(threading.Thread):
    def __init__(self, *args, **kwargs):
        super(AutonomousThread, self).__init__(*args, **kwargs)
        super().__setattr__('daemon', True)
        self._stop_event = threading.Event()
        self.robot = None
        self.endAction = lambda: None

    def passRobot(self, robot):
        self.robot = robot
    
    def withEndAction(self, action):
        self.endAction = action

    def stop(self):
        self._stop_event.set()
    
    def stopped(self):
        return self._stop_event.is_set()

