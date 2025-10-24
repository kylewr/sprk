#pragma once

#include "robot/base/RobotBase.hpp"
#include "robot/base/SocketManager.hpp"

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
};
