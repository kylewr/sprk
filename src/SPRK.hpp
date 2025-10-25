#pragma once

#include "src/base/RobotBase.hpp"
#include "src/base/SocketManager.hpp"

#include "subsystem/Drivetrain.hpp"
#include "subsystem/Pinchers.hpp"

class SPRK : public RobotBase {
    public:
        SPRK(SocketManagerArgs* socketArgs);
        ~SPRK() = default;

        void loop();
    
    private:    
        Drivetrain* drivetrain;
        Pinchers* pinchers;

        void handleIncomingMessage(const std::string& msg);
        void handleTeleopPacket(const std::string& packet);
};
