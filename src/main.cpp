#include <iostream>
#include <memory>
#include <thread>

#include "SPRK.hpp"
#include "Constants.hpp"

std::unique_ptr<SPRK> robot;

int main() {
    SPRKArgs* args = new SPRKArgs();

    args->ipAddress = Constants::SPRKConstants::IP_ADDRESS;
    args->portNumber = Constants::SPRKConstants::PORT;

    robot = std::make_unique<SPRK>(args);

    robot->run();
    
    return 0;
}
