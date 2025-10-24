#include "SPRK.hpp"

SPRK::SPRK(SocketManagerArgs* socketArgs) : RobotBase(socketArgs)
{
    socketArgs->incomingMessageHandler = [this](const std::string& msg) {
        this->telemetry.log("Incoming message: " + msg, LogLevel::INFO);
    };

    drivetrain = new Drivetrain();
    pinchers = new Pinchers();

    addSubsystem({drivetrain, pinchers});
}