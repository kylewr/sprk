#include <iostream>
#include <memory>
#include <thread>

#include "SPRK.hpp"

// #include <pigpio.h>

std::unique_ptr<SPRK> robot;

int main() {
    SPRKArgs* args = new SPRKArgs();

    args->ipAddress = "0.0.0.0";
    args->portNumber = 8007;

    robot = std::make_unique<SPRK>(args);

    // robot->loop();

    while (robot->isAlive()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (robot->getCurrentState() == RobotState::TELEOP) {
            robot->getTelemetry().log("Main loop iteration. Meow.", LogLevel::INFO);
            std::this_thread::sleep_for(std::chrono::milliseconds(1989));
        }
    }

    return 0;
}
