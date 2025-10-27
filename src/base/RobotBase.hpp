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
        virtual ~RobotBase();

        virtual void changeState(RobotState newState) final;

        virtual void run() final;

        virtual bool autonomousInit() {
            return true;
        };

        virtual bool teleopInit() {
            return true;
        };

        virtual void disabledInit() {};

        virtual RobotState getCurrentState() const final {
            return currentState;
        }

        virtual bool isAlive() const final {
            return alive;
        }

        virtual bool isSimulation() const final {
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

        virtual RobotInfoArgs* getInfoArgs() const final {
            return infoArgs;
        }
        virtual void setInfoArgs(RobotInfoArgs* args) final;

        void addSubsystem(Subsystem* subsystem);
        void addSubsystem(std::initializer_list<Subsystem*> newSubsystems);

        bool setSocketArguments(SocketManagerArgs* args);

        virtual void handleTeleopPacket(const std::string& packet);
        virtual void addJoystickButtons() {};

        virtual void loop() {
            // default does nothing
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        };

    private:
        RobotInfoArgs* infoArgs {nullptr};
        RobotState currentState {RobotState::DISABLED};

        void initSocketArgs();
        void informControllerInit();
        void handleIncomingMessage(const std::string& msg);
};
