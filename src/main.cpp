#include <iostream>
#include <memory>
#include <thread>

#include "SPRK.hpp"

std::unique_ptr<SPRK> robot;

int main() {
    SPRKArgs* args = new SPRKArgs();

    args->ipAddress = "0.0.0.0";
    args->portNumber = 8007;

    robot = std::make_unique<SPRK>(args);

    robot->run();
    
    return 0;
}
