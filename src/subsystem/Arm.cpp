#include "Arm.hpp"

Arm::Arm(SerialInterface* serial) : Subsystem("ARM"), serialInterface(serial) {
    stepperManager = new VirtualStepperManager(serialInterface);

    turret = new VirtualStepper(Constants::IOMap::TURRET_ID);
    arm = new VirtualStepper(Constants::IOMap::ARM_ID);
    wrist = new VirtualStepper(Constants::IOMap::WRIST_ID);

    stepperManager->addStepper({turret, arm, wrist});
    stepperManager->disable(); // ensure disabled
}

Arm::~Arm() {
    delete stepperManager;
    delete turret;
    delete arm;
    delete wrist;
}

void Arm::changedState(RobotState newState) {
    switch (newState) {
        case RobotState::DISABLED:
            stepperManager->disable();
            break;
        case RobotState::AUTONOMOUS:
        case RobotState::TELEOP:
            stepperManager->enable();
            break;
        default:
            stepperManager->disable();
            break;
    }
}
