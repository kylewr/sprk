#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "RobotEnums.hpp"
#include "RobotHelpers.hpp"
#include "SocketManager.hpp"
#include "Subsystem.hpp"
#include "Telemetry.hpp"

class RobotBase {
    public:
        RobotBase();
        ~RobotBase();

        void changeState(RobotState newState);

        void autonomousInit();

        void teleopInit();

        void disabledInit();

        RobotState getCurrentState() const {
            return currentState;
        }
        
        bool isSimulation() const {
            return simulation;
        }

        SocketManager& getSocketManager() {
            return socketManager;
        }

        RobotTelemetry& getTelemetry() {
            return telemetry;
        }

    protected:
        bool alive {true};
        bool simulation {false};

        SocketManagerArgs* socketArgs {nullptr};
        SocketManager socketManager;

        RobotTelemetry telemetry;
        std::vector<Subsystem*> subsystems;

        void addSubsystem(Subsystem* subsystem);
        void addSubsystem(std::initializer_list<Subsystem*> newSubsystems);

        bool setSocketArguments(SocketManagerArgs* args);

        void informControllerInit(RobotInfoArgs* args);

    private:
        RobotState currentState {RobotState::DISABLED};

        void initSocketArgs();
};
