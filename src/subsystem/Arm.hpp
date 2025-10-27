#pragma once

#include "Constants.hpp"

#include "base/RobotEnums.hpp"
#include "base/Subsystem.hpp"
#include "base/SerialInterface.hpp"
#include "base/actuation/VirtualStepper.hpp"
#include "base/actuation/VirtualStepperManager.hpp"

class Arm : public Subsystem {
    public:
        Arm(SerialInterface* serial);
        ~Arm();

        VirtualStepperManager* getStepperManager() const {
            return stepperManager;
        }

        void changedState(RobotState newState) override;

    private:
        SerialInterface* serialInterface;
        VirtualStepperManager* stepperManager;

        VirtualStepper* turret;
        VirtualStepper* arm;
        VirtualStepper* wrist;
};
