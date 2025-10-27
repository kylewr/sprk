#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "src/base/Joystick.hpp"
#include "src/base/RobotBase.hpp"
#include "src/base/SerialInterface.hpp"
#include "src/base/SocketManager.hpp"
#include "src/base/actuation/VirtualStepperManager.hpp"

#include "subsystem/Arm.hpp"
#include "subsystem/Drivetrain.hpp"
#include "subsystem/Pinchers.hpp"

class SPRKArgs {
    public:
        SPRKArgs() = default;

        std::string ipAddress;
        uint16_t portNumber;
};

class SPRK final : public RobotBase {
    public:
        SPRK(SPRKArgs* args);
        ~SPRK() = default;

        // bool autonomousInit() override;
        // bool teleopInit() override;
        // void disabledInit() override;

        std::vector<std::string> getAutonNames() const {
            return {"MeowAuton1", "MeowAuton2"};
        }

        SerialInterface& getSerialInterface() {
            return *serialInterface;
        }

    private:
        SPRKArgs* sprkArgs;

        Arm* arm;
        Drivetrain* drivetrain;
        Pinchers* pinchers;

        SerialInterface* serialInterface;

        void loop() override;

        void addJoystickButtons() override;
        // void handleTeleopPacket(const std::string& packet) override;
};
