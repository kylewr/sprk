from os import path

class SimState:
    IS_SIM = None
    SIM_SERIAL = False

    @staticmethod
    def isSimulation():
        if SimState.IS_SIM is None:
            SimState.IS_SIM = path.exists("../.simulation")
        return SimState.IS_SIM
    
    @staticmethod
    def getBuildTime():
        try:
            with open("./_DEPLOYTIME.txt", 'r') as f:
                return f.read().strip()
        except FileNotFoundError:
            return "Unknown Build Time"
