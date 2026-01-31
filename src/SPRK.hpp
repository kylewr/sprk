#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "RobotCamera.hpp"

#include "src/base/Joystick.hpp"
#include "src/base/RobotBase.hpp"
#include "src/base/SerialInterface.hpp"
#include "src/base/SocketManager.hpp"
#include "src/base/Trigger.hpp"
#include "src/base/actuation/VirtualStepperManager.hpp"
#include "src/base/spi/RobotSPI.hpp"

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
        explicit SPRK(SPRKArgs* args);
        ~SPRK();

        bool autonomousInit() override;
        bool teleopInit() override;
        void disabledInit() override;

        bool attemptEnable();

        std::vector<std::string> getAutonNames() const {
            return {"MeowAuton1", "MeowAuton2"};
        }

        SerialInterface& getSerialInterface() {
            return *serialInterface;
        }

    private:
        SPRKArgs* sprkArgs;

        RobotSPI robotSPI;
        SerialInterface* serialInterface;

        Arm* arm;
        Drivetrain* drivetrain;
        Pinchers* pinchers;

        RobotCamera* camera;

        void loop() override;

        void addJoystickAxies();
        void addTriggers();
};
