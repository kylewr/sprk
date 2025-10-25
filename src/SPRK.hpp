#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "src/base/RobotBase.hpp"
#include "src/base/SocketManager.hpp"

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

        void loop();

        std::vector<std::string> getAutonNames() const {
            return {"MeowAuton1", "MeowAuton2"};
        }

    private:
        SPRKArgs* sprkArgs;

        Drivetrain* drivetrain;
        Pinchers* pinchers;

        void handleIncomingMessage(const std::string& msg);
        void handleTeleopPacket(const std::string& packet);
};
