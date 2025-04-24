from os import path

class SimState:
    IS_SIM = None

    @staticmethod
    def isSimulation():
        if SimState.IS_SIM is None:
            SimState.IS_SIM = path.exists("../.simulation")
        return SimState.IS_SIM


