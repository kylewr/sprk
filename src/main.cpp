#include <iostream>
#include <memory>
#include <thread>

#include "SPRK.hpp"

std::unique_ptr<SPRK> robot;

int main() {
    SPRKArgs* args = new SPRKArgs();

    args->ipAddress = "192.168.1.31";
    args->portNumber = 8007;

    robot = std::make_unique<SPRK>(args);

    robot->loop();

    return 0;
}
