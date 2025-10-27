#include "VirtualStepperManager.hpp"

#include "StepperConstants.hpp"

void VirtualStepperManager::addStepper(VirtualStepper* stepper) {
    if (serialInterface == nullptr) {
        return;
    }

    for (auto& existingStepper : steppers) {
        if (existingStepper->getId() == stepper->getId()) {
            return;
        }
    }
    stepper->provideSerialInterface(serialInterface);

    steppers.push_back(stepper);
}

void VirtualStepperManager::addStepper(std::vector<VirtualStepper*> steppers) {
    for (auto& stepper : steppers) {
        addStepper(stepper);
    }
}

bool VirtualStepperManager::enable() {
    if (!serialInterface->getIsOpen()) {
        return false;
    }

    return serialInterface->writeData(VirtualStepperConstants::ENABLE_CMD);
}

bool VirtualStepperManager::disable() {
    if (!serialInterface->getIsOpen()) {
        return false;
    }

    return serialInterface->writeData(VirtualStepperConstants::DISABLE_CMD);
}
