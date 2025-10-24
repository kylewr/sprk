#pragma once

#include <vector>
#include <memory>
#include <initializer_list>

#include "SocketManager.hpp"
#include "Subsystem.hpp"
#include "RobotEnums.hpp"
#include "Telemetry.hpp"

class RobotBase {
    public:
        RobotBase(SocketManagerArgs* socketArgs = nullptr);
        ~RobotBase();

        void changeState(RobotState newState);

        void autonomousInit();

        void teleopInit();

        void disabledInit();

        SocketManager& getSocketManager() {
            return socketManager;
        }

        RobotTelemetry& getTelemetry() {
            return telemetry;
        }
    
    protected:
        bool alive {true};

        SocketManager socketManager;

        RobotState currentState {RobotState::DISABLED};
        RobotTelemetry telemetry;
        std::vector<Subsystem*> subsystems;

        void addSubsystem(Subsystem* subsystem);
        void addSubsystem(std::initializer_list<Subsystem*> newSubsystems);

};
