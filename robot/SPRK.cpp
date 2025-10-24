#include "SPRK.hpp"
#include <thread>

SPRK::SPRK(SocketManagerArgs* socketArgs) : RobotBase(socketArgs)
{
    socketArgs->incomingMessageHandler = [this](const std::string& msg) {
        this->telemetry.log("Incoming message: " + msg, LogLevel::INFO);
    };

    bool success = socketManager.initializeSocket();
    if (!success) {
        std::cout << "Socket initialized unsuccessfully.\n";
    } else {
        std::cout << "Socket initialized successfully.\n";
    }

    drivetrain = new Drivetrain();
    pinchers = new Pinchers();

    addSubsystem({drivetrain, pinchers});
}

void SPRK::loop()
{
    while (alive) {
        // Main loop logic here
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        telemetry.log("SPRK main loop iteration. Meow.", LogLevel::INFO);
    }
}
