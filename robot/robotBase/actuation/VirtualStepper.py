from robotBase.simulation.SimState import SimState

"""
Defines a virtual stepper which goes through
an arduino to contrlo the stepper motor.
"""
class VirtualStepper:
    def __init__(self, id: int) -> None:
        self.id = id

        self.serial = None
    
    def pass_serial(self, serial) -> None:
        self.serial = serial
    
    def setEnabled(self, enabled: bool) -> None:
        pass

    def setAngle(self, angle: float) -> None:
        pass

    def setRPM(self, rps: float) -> None:
        pass

    def rotateContinuous(self, rps: float) -> None:
        pass

if SimState.isSimulation():
    from robotBase.simulation.SerialSim import SerialSim as Serial
else:
    from robotBase.SerialBase import Serial