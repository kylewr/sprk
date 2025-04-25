class GPIOSim:
    LOW = 0
    HIGH = 1

    OUT = 0
    IN = 1

    BOARD = 10
    BCM = 11

    logging = True

    @staticmethod
    def sim_setLogging(bool: bool) -> None:
        GPIOSim.logging = bool

    @staticmethod
    def setmode(mode) -> None:
        if GPIOSim.logging:
            print("[SIM_GPIO] `setmode` called with mode:", mode)

    @staticmethod
    def setup(pin, mode) -> None:
        if GPIOSim.logging:
            print("[SIM_GPIO] `setup` called with pin:", pin, "and mode:", mode)
    
    @staticmethod
    def output(pin, state) -> None:
        if GPIOSim.logging:
            print("[SIM_GPIO] `output` called with pin:", pin, "and state:", state)