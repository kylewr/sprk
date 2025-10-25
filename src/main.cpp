#include <iostream>
#include <memory>
#include <thread>

#include "SPRK.hpp"
#include "base/SocketManager.hpp"

std::unique_ptr<SPRK> robot;

int main() {
    // TODO: convert this into sprk args or smth
    SocketManagerArgs* socketArgs = new SocketManagerArgs();

    socketArgs->ipAddress  = "0.0.0.0";
    socketArgs->portNumber = 8008;

    robot = std::make_unique<SPRK>(socketArgs);

    robot->loop();

    return 0;
}
