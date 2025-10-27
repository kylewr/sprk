#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

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

class SPRK : public RobotBase {
    public:
        SPRK(SPRKArgs* args);
        ~SPRK() = default;

        void loop() override;

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

        std::vector<std::function<void()>> teleopSubscribers;

        void handleIncomingMessage(const std::string& msg);
        void handleTeleopPacket(const std::string& packet);
};
