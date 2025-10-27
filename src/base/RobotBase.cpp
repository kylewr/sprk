#include "RobotBase.hpp"

#include <algorithm>
#include <iostream>
#include <string>

RobotBase::RobotBase() : telemetry(RobotTelemetry()) {
    telemetry.log("RobotBase initialized.", LogLevel::INFO, true);

    changeState(RobotState::DISABLED);

    telemetry.log("Completed RobotBase Initialization.", LogLevel::SUCCESS, true);
}

RobotBase::~RobotBase() {
    telemetry.log("RobotBase cleaning up.", LogLevel::INFO);

    if (getCurrentState() != RobotState::DISABLED) {
        changeState(RobotState::DISABLED);
    }

    for (Subsystem* subsystem : subsystems) {
        delete subsystem;
    }
}

void RobotBase::changeState(RobotState newState) {
    currentState = newState;

    switch (newState) {
        case RobotState::AUTONOMOUS:
            if (!autonomousInit()) {
                telemetry.log("Autonomous initialization failed.", LogLevel::ERROR);
                return;
            }
            break;
        case RobotState::TELEOP:
            if (!teleopInit()) {
                telemetry.log("Teleop initialization failed!", LogLevel::ERROR);
                return;
            }
            break;
        case RobotState::DISABLED:
            disabledInit();
            break;
    }

    for (Subsystem* subsystem : subsystems) {
        subsystem->changedState(newState);
    }

    telemetry.logRobotState(newState); // also reports the state to the controller
}

void RobotBase::addSubsystem(Subsystem* subsystem) {
    if (subsystem == nullptr) {
        telemetry.log("Attempted to add null subsystem!", LogLevel::ERROR);
        return;
    }

    subsystems.push_back(subsystem);

    subsystem->getTelemetry()->provideRobotTelemetry(&telemetry);
}

void RobotBase::addSubsystem(std::initializer_list<Subsystem*> newSubsystems) {
    for (Subsystem* subsystem : newSubsystems) {
        addSubsystem(subsystem);
    }
}

bool RobotBase::setSocketArguments(SocketManagerArgs* args) {
    socketArgs = args;
    bool status = socketManager.setArgs(args);
    if (!status) {
        telemetry.log("Failed to set socket arguments; socket already initialized.",
                      LogLevel::ERROR, true);
    } else {
        initSocketArgs();
    }
    return status;
}

void RobotBase::informControllerInit(RobotInfoArgs* args) {
    if (args == nullptr) {
        telemetry.log("RobotInfoArgs is null; cannot inform controller of init.", LogLevel::ERROR);
        return;
    }
    if (socketManager.hasConnection()) {
        if (isSimulation() && std::find(args->flags.begin(), args->flags.end(),
                                        RobotFlags::SIMULATION) == args->flags.end()) {
            args->flags.push_back(RobotFlags::SIMULATION);
        }

        bool success = socketManager.sendMessage(args->str());
        if (!success) {
            telemetry.log("Failed to send RobotInfoArgs to controller.", LogLevel::ERROR);
        }
    } else {
        telemetry.log("No socket connection; cannot inform controller of init.", LogLevel::WARN);
    }
}

void RobotBase::initSocketArgs() {
    if (socketArgs == nullptr) {
        telemetry.log("Socket arguments not set; cannot initialize socket.", LogLevel::ERROR, true);
        return;
    }

    // Tell socket manager to send meta messages to telemetry
    if (socketArgs->socketMessageHandler == nullptr) {
        socketArgs->socketMessageHandler = [this](const std::string& msg, LogLevel level) {
            this->telemetry.log(msg, level, true);
        };
    }

    telemetry.setSocketSupplier([this](const std::string& msg) {
        if (!this->socketManager.hasConnection()) {
            return;
        }

        bool success = this->socketManager.sendMessage(msg);
        if (!success && this->socketManager.isInitialized()) {
            std::cout << "Failed to send message to socket: " << msg << '\n';
        }
    });
}
