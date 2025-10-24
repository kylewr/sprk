#include "RobotBase.hpp"
#include <string>
#include <iostream>

RobotBase::RobotBase(SocketManagerArgs* socketArgs) : telemetry(RobotTelemetry()), socketManager(socketArgs)
{
    telemetry.log("RobotBase initialized.", LogLevel::INFO);
    
    if (socketArgs != nullptr) {
        if (socketArgs->socketMessageHandler == nullptr) {
            socketArgs->socketMessageHandler = [this](const std::string& msg, LogLevel level) {
                this->telemetry.log(msg, level);
            };
        }
    }
    
    telemetry.setSocketSupplier([this](const std::string& msg) {
        bool success = this->socketManager.sendMessage(msg);
        if (!success) {
            this->telemetry.log("Failed to send message over socket: " + msg, LogLevel::ERROR);
        }
    });

    changeState(RobotState::DISABLED);

    telemetry.log("Completed RobotBase Initialization.", LogLevel::SUCCESS);
}

RobotBase::~RobotBase()
{
    telemetry.log("RobotBase cleaning up.", LogLevel::INFO);
    for (Subsystem* subsystem : subsystems)
    {
        delete subsystem;
    }
    
}

void RobotBase::changeState(RobotState newState)
{
    currentState = newState;
    telemetry.logRobotState(newState);

    switch (newState)
    {
    case RobotState::AUTONOMOUS:
        autonomousInit();
        break;
    case RobotState::TELEOP:
        teleopInit();
        break;
    case RobotState::DISABLED:
        disabledInit();
        break;
    }
}

void RobotBase::autonomousInit()
{
    // TODO: Implement
}

void RobotBase::teleopInit()
{
    // TODO: Implement
}

void RobotBase::disabledInit()
{
    // TODO: Implement
}

void RobotBase::addSubsystem(Subsystem *subsystem)
{
    if (subsystem == nullptr)
    {
        telemetry.log("Attempted to add null subsystem!", LogLevel::ERROR);
        return;
    }

    subsystems.push_back(subsystem);

    subsystem->getTelemetry()->provideRobotTelemetry(&telemetry);
}

void RobotBase::addSubsystem(std::initializer_list<Subsystem *> newSubsystems)
{
    for (Subsystem *subsystem : newSubsystems)
    {
        addSubsystem(subsystem);
    }
}
