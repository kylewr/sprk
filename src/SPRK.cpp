#include "SPRK.hpp"

#include <algorithm>
#include <thread>

#include "base/RobotHelpers.hpp"

SPRK::SPRK(SPRKArgs* args) : RobotBase(), sprkArgs(args) {
    RobotInfoArgs* infoArgs = new RobotInfoArgs();
    infoArgs->message = "Welcome to the C++ SPRK Robot!";
    infoArgs->autons = this->getAutonNames();
    infoArgs->flags.push_back(RobotFlags::CAMERA);
    
    setInfoArgs(infoArgs);

    SocketManagerArgs* socketArgs = new SocketManagerArgs();
    socketArgs->ipAddress = sprkArgs->ipAddress;
    socketArgs->portNumber = sprkArgs->portNumber;

    setSocketArguments(socketArgs);

    socketManager.onConnect([this]() {
        this->changeState(RobotState::DISABLED);
    });

    bool success = socketManager.initializeSocket();
    if (!success) {
        telemetry.log("Failed to initialize socket.", LogLevel::ERROR, true);
    }

    serialInterface =
        new SerialInterface(Constants::IOMap::SERIAL_PORT, Constants::IOMap::BAUD_RATE);
    serialInterface->onReceive([this](const std::string& msg) {
        this->telemetry.log("Received serial message: " + msg, LogLevel::VERBOSE);
    });

    if (serialInterface->openPort()) {
        telemetry.log("Serial port opened on " + std::string(Constants::IOMap::SERIAL_PORT) +
                          " opened.",
                      LogLevel::INFO);
    } else {
        telemetry.log("Failed to open serial port on " +
                          std::string(Constants::IOMap::SERIAL_PORT) + "!",
                      LogLevel::ERROR);
    }

    arm = new Arm(serialInterface);
    drivetrain = new Drivetrain();
    pinchers = new Pinchers();

    addSubsystem({arm, drivetrain, pinchers});
}

void SPRK::loop() {
    while (alive) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


void SPRK::handleTeleopPacket(const std::string& packet) {}
