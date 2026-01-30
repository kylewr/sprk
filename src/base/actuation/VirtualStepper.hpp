#pragma once

#include "base/SerialInterface.hpp"
#include "base/actuation/StepperConstants.hpp"

class VirtualStepper {
    public:
        VirtualStepper(int id) : id(id) {}
        ~VirtualStepper() = default;

        int getId() const {
            return id;
        }
        
        void provideSerialInterface(SerialInterface* serial) {
            serialInterface = serial;
            hasSerialInterface = true;
        }

        void rotateContinuous(StepperDirection direction);

    private:
        int id;

        SerialInterface* serialInterface;
        bool hasSerialInterface = false;

        int angle {0};
        int rpm {120};
        bool continuous {false};
        
};