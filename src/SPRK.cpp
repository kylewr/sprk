#include "SPRK.hpp"

#include <algorithm>
#include <thread>

#include "base/RobotHelpers.hpp"

SPRK::SPRK(SPRKArgs* args) : RobotBase(), sprkArgs(args) {
    SocketManagerArgs* socketArgs = new SocketManagerArgs();
    socketArgs->ipAddress = sprkArgs->ipAddress;
    socketArgs->portNumber = sprkArgs->portNumber;

    socketArgs->incomingMessageHandler = [this](const std::string& msg) {
        this->handleIncomingMessage(msg);
    };

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

void SPRK::handleIncomingMessage(const std::string& msg) {
    if (msg.compare(0, 4, "exit") == 0) {
        socketManager.closeSocket();
        changeState(RobotState::DISABLED);
        alive = false;
    } else if (msg.compare(0, 5, "init,") == 0) {
        std::string controllerVersion = msg.substr(5);

        RobotInfoArgs* infoArgs = new RobotInfoArgs();
        infoArgs->message = "Welcome to the C++ SPRK Robot!";
        infoArgs->autons = this->getAutonNames();
        infoArgs->flags.push_back(RobotFlags::CAMERA);

        informControllerInit(infoArgs);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(50)); // Sleep for 50ms to ensure message is sent

        telemetry.log("Received controller version: " + controllerVersion, LogLevel::INFO);
    } else if (msg.compare(0, 4, "tele") == 0) {
        changeState(RobotState::TELEOP);
    } else if (msg.compare(0, 4, "auto") == 0) {
        changeState(RobotState::AUTONOMOUS);
    } else if (msg.compare(0, 3, "dis") == 0) {
        changeState(RobotState::DISABLED);
    } else if (msg.compare(0, 3, "te-") == 0) {
        handleTeleopPacket(msg);
    } else {
        telemetry.log("Unknown incoming message: " + msg, LogLevel::ERROR);
    }
}

void SPRK::handleTeleopPacket(const std::string& packet) {}
