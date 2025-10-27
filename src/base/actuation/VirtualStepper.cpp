#include "VirtualStepper.hpp"

void VirtualStepper::rotateContinuous(StepperDirection direction) {
    if (!hasSerialInterface || serialInterface == nullptr) {
        return;
    }

    std::string command = std::to_string(id);

    switch (direction) {
        case StepperDirection::CW:
            command += "rcw" SERIAL_END_BYTE;
            break;
        case StepperDirection::CCW:
            command += "rccw" SERIAL_END_BYTE;
            break;
        case StepperDirection::STOP:
            command += "s" SERIAL_END_BYTE;
            break;
    }

    serialInterface->writeData(command);
}
