import subprocess

from robotBase.simulation.SimState import SimState

class RobotCamera:
    def __init__(self):
        self.cameraProcess = None
        self.cameraCommand = 'mjpg_streamer -i "input_uvc.so -d /dev/video0 -r 1280x1024 -f 20 -ex 960 -gain 8" -o "output_http.so"'

    def start(self):
        if (SimState.isSimulation()):
            print("Camera started [simulation].")
            return
        if self.cameraProcess is not None and self.cameraProcess.poll() is None:
            print("Camera is already running.")
            return
        self.cameraProcess = subprocess.Popen(self.cameraCommand, shell=True)
        print("Camera started.")

    def stop(self):
        if (SimState.isSimulation()):
            print("Camera started [simulation].")
            return
        if self.cameraProcess is not None:
            try:
                self.cameraProcess.terminate()
                self.cameraProcess.wait(timeout=5)
                subprocess.run(["pkill", "-f", "mjpg_streamer"], timeout=5)
            except Exception:
                self.cameraProcess.kill()
                print("Camera process killed due to timeout.")
            print("Camera stopped.")
        else:
            print("No camera process to stop.")