#pragma once

#include <vector>

#include "src/base/SerialInterface.hpp"
#include "src/base/actuation/VirtualStepper.hpp"

class VirtualStepperManager {
    public:
        VirtualStepperManager(SerialInterface* serialInterface) : serialInterface(serialInterface) {};
        ~VirtualStepperManager() = default;

        void addStepper(VirtualStepper* stepper);
        void addStepper(std::vector<VirtualStepper*> steppers);

        bool enable();
        bool disable();

    private:
        SerialInterface* serialInterface;

        std::vector<VirtualStepper*> steppers;
};
