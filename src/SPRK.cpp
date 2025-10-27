#include "SPRK.hpp"

#include <algorithm>
#include <thread>

#include "base/RobotHelpers.hpp"
#include "base/simulation/SerialSimulation.hpp"

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

    if (!socketManager.initializeSocket()) {
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
                          std::string(Constants::IOMap::SERIAL_PORT) + "! Attempting simulation.",
                      LogLevel::ERROR);
        
        delete serialInterface;

        serialInterface = new SerialSimulation(Constants::IOMap::SERIAL_PORT, Constants::IOMap::BAUD_RATE);
        serialInterface->onReceive([this](const std::string& msg) {
            this->telemetry.log("Received serial message: " + msg, LogLevel::VERBOSE);
        });

        serialInterface->openPort();

        telemetry.log("Serial simulation interface initialized.", LogLevel::INFO);
    }

    arm = new Arm(serialInterface);
    drivetrain = new Drivetrain();
    pinchers = new Pinchers();

    addSubsystem({arm, drivetrain, pinchers});

    addJoystickButtons();
}

void SPRK::loop() {
    while (alive) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void SPRK::addJoystickButtons() {
    teleopInstructions[JoystickButton::LEFTSHOULDER] = [this]() {
        this->telemetry.log("LEFTSHOULDER pressed; moving turret CW.", LogLevel::INFO);
        this->arm->moveTurret(StepperDirection::CW);
    };
    teleopInstructions[JoystickButton::_LEFTSHOULDER] = [this]() {
        this->telemetry.log("LEFTSHOULDER unpressed; stopping turret.", LogLevel::INFO);
        this->arm->moveTurret(StepperDirection::STOP);
    };
}

// void SPRK::handleTeleopPacket(const std::string& packet) {
//     telemetry.log("Received teleop packet: " + packet, LogLevel::INFO);
// }
