from time import sleep

def run(robot):
    robot.telemetry.log("Autonomous mode started.")
    i = 0
    while True:
        i += 1
        robot.telemetry.log(f"Autonomous loop iteration: {i}")
        sleep(1)
