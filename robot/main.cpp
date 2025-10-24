#include <iostream>
#include <thread>
#include <memory>

#include "SPRK.hpp"
#include "base/SocketManager.hpp"

std::unique_ptr<SPRK> robot;

int main() {
    // TODO: convert this into sprk args or smth
    SocketManagerArgs* socketArgs = new SocketManagerArgs();

    socketArgs->ipAddress = "192.168.1.31";
    socketArgs->portNumber = 8008;

    robot = std::make_unique<SPRK>(socketArgs);

    robot->loop();

    std::cout << "Exiting main." << std::endl;

    // robot->getTelemetry().setGlobalVerbose(true);

    // RobotTelemetry& telemetry = robot->getTelemetry();
    // telemetry.log("test info", LogLevel::INFO);
    // telemetry.log("test success", LogLevel::SUCCESS);
    // telemetry.log("test warning", LogLevel::WARN);
    // telemetry.log("test error", LogLevel::ERROR);

    return 0;
}
