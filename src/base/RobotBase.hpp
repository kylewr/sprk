#pragma once

#include <initializer_list>
#include <memory>
#include <thread>
#include <vector>

#include "Joystick.hpp"
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

        virtual void loop() {
            // default does nothing
            while (alive) {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        };

        virtual bool autonomousInit() {
            return true;
        };

        virtual bool teleopInit() {
            return true;
        };

        virtual void disabledInit() {};

        RobotState getCurrentState() const {
            return currentState;
        }

        bool isAlive() const {
            return alive;
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

        std::unordered_map<JoystickButton, std::function<void()>> teleopInstructions {};

        RobotInfoArgs* getInfoArgs() const {
            return infoArgs;
        }
        void setInfoArgs(RobotInfoArgs* args);

        void addSubsystem(Subsystem* subsystem);
        void addSubsystem(std::initializer_list<Subsystem*> newSubsystems);

        bool setSocketArguments(SocketManagerArgs* args);

        virtual void handleTeleopPacket(const std::string& packet);
        virtual void addJoystickButtons() {};

    private:
        RobotInfoArgs* infoArgs {nullptr};
        RobotState currentState {RobotState::DISABLED};

        void initSocketArgs();
        void informControllerInit();
        void handleIncomingMessage(const std::string& msg);
};
