#include "SPRK.hpp"
#include <algorithm>
#include <thread>

SPRK::SPRK(SPRKArgs* args) : RobotBase() {
    sprkArgs = args;

    SocketManagerArgs* socketArgs = new SocketManagerArgs();
    socketArgs->ipAddress         = sprkArgs->ipAddress;
    socketArgs->portNumber        = sprkArgs->portNumber;

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
    } else {
        telemetry.log("Socket initialized successfully.", LogLevel::SUCCESS, true);
    }

    drivetrain = new Drivetrain();
    pinchers   = new Pinchers();

    addSubsystem({drivetrain, pinchers});
}

void SPRK::loop() {
    while (alive) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (getCurrentState() == RobotState::TELEOP) {
            telemetry.log("SPRK main loop iteration. Meow.", LogLevel::WARN);
            std::this_thread::sleep_for(std::chrono::milliseconds(1989));
        }
    }
}

void SPRK::handleIncomingMessage(const std::string& msg) {
    if (msg.compare(0, 4, "exit") == 0) {
        this->socketManager.closeSocket();
        this->changeState(RobotState::DISABLED);
        this->alive = false;
    } else if (msg.compare(0, 5, "init,") == 0) {
        std::string controllerVersion = msg.substr(5);

        this->telemetry.log("Received init command from controller.", LogLevel::INFO);
        this->telemetry.log("Controller version: " + controllerVersion, LogLevel::SUCCESS);

        std::string response = "[ROBOTINFO] \nSPRK Robot v1.0 "
                               "Initialized.[SIG][AUTONS]Meow,Meow2[SIG][FLAGS],camera,\n";
        this->socketManager.sendMessage(response);
    } else if (msg.compare(0, 4, "tele") == 0) {
        this->changeState(RobotState::TELEOP);
    } else if (msg.compare(0, 4, "auto") == 0) {
        this->changeState(RobotState::AUTONOMOUS);
    } else if (msg.compare(0, 3, "dis") == 0) {
        this->changeState(RobotState::DISABLED);
    } else if (msg.compare(0, 3, "te-") == 0) {
        // handle teleop packets here
    } else {
        this->telemetry.log("Unknown incoming message: " + msg, LogLevel::ERROR);
    }
}

void SPRK::handleTeleopPacket(const std::string& packet) {}
