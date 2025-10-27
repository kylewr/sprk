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
        telemetry.log("The robot is being destroyed while not disabled!", LogLevel::WARN);
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

void RobotBase::setInfoArgs(RobotInfoArgs* args) {
    if (infoArgs == nullptr) {
        infoArgs = args;
    } else {
        telemetry.log("RobotInfoArgs already set; cannot set again.", LogLevel::ERROR);
    }
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

void RobotBase::initSocketArgs() {
    if (socketArgs == nullptr) {
        telemetry.log("Socket arguments not set; cannot initialize socket.", LogLevel::ERROR, true);
        return;
    }

    // Tell socket manager to send meta messages to telemetry
    if (socketArgs->socketMessageHandler == nullptr) {
        // messages from the socket manager
        socketArgs->socketMessageHandler = [this](const std::string& msg, LogLevel level) {
            this->telemetry.log(msg, level, true);
        };

        // messages from remote connection
        socketArgs->incomingMessageHandler = [this](const std::string& msg) {
            this->handleIncomingMessage(msg);
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

void RobotBase::informControllerInit() {
    if (infoArgs == nullptr) {
        telemetry.log("RobotInfoArgs is null; cannot inform controller of init.", LogLevel::ERROR);
        return;
    }
    if (socketManager.hasConnection()) {
        if (isSimulation() && std::find(infoArgs->flags.begin(), infoArgs->flags.end(),
                                        RobotFlags::SIMULATION) == infoArgs->flags.end()) {
            infoArgs->flags.push_back(RobotFlags::SIMULATION);
        }

        bool success = socketManager.sendMessage(infoArgs->str());
        if (!success) {
            telemetry.log("Failed to send RobotInfoArgs to controller.", LogLevel::ERROR);
        }
    } else {
        telemetry.log("No socket connection; cannot inform controller of init.", LogLevel::WARN);
    }
}

void RobotBase::handleIncomingMessage(const std::string& msg) {
    // TODO: convert string literals to constants

    if (msg.compare(0, 4, "exit") == 0) {
        socketManager.closeSocket();
        changeState(RobotState::DISABLED);
        alive = false;
    } else if (msg.compare(0, 5, "init,") == 0) {
        std::string controllerVersion = msg.substr(5);

        informControllerInit();

        std::this_thread::sleep_for(
            std::chrono::milliseconds(50)); // Sleep for 50ms to ensure message is sent

        telemetry.log("Received controller version: " + controllerVersion, LogLevel::INFO);
    } else if (msg.compare(0, 4, "tele") == 0) {
        changeState(RobotState::TELEOP);
    } else if (msg.compare(0, 4, "auto") == 0) {
        changeState(RobotState::AUTONOMOUS);
    } else if (msg.compare(0, 3, "dis") == 0) {
        changeState(RobotState::DISABLED);
    } else if (msg.compare(0, 3, "te-") == 0) {
        handleTeleopPacket(msg);
    } else {
        telemetry.log("Unknown incoming message: " + msg, LogLevel::ERROR);
    }
}
