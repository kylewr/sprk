#include "SPRK.hpp"

#include <algorithm>
#include <thread>

#include "SPIMappings.hpp"
#include "base/RobotHelpers.hpp"
#include "base/Trigger.hpp"
#include "base/simulation/SerialSimulation.hpp"

SPRK::SPRK(SPRKArgs* args) : RobotBase(), sprkArgs(args), robotSPI(0, 8, 1000000, 0, true) {
    try {
        robotSPI.initialize();
    } catch (const std::runtime_error& e) {
        telemetry.log(std::string("Failed to initialize SPI: ") + e.what(), LogLevel::ERROR, true);
        telemetry.log("Continuing in simulation mode.", LogLevel::WARN, true);
    }

    registerJoystick(new SocketXBoxController());

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

        serialInterface =
            new SerialSimulation(Constants::IOMap::SERIAL_PORT, Constants::IOMap::BAUD_RATE);
        serialInterface->onReceive([this](const std::string& msg) {
            this->telemetry.log("Received serial message: " + msg, LogLevel::VERBOSE);
        });

        serialInterface->openPort();

        telemetry.log("Serial simulation interface initialized.", LogLevel::INFO);
    }

    arm = new Arm(serialInterface, &robotSPI);
    drivetrain = new Drivetrain();
    pinchers = new Pinchers(&robotSPI);

    addSubsystem({arm, drivetrain, pinchers});

    addJoystickButtons();
}

bool SPRK::autonomousInit() {
    static const uint8_t initData[16] = {identToByte(COMMAND_IDENT::ROBOT_ENABLE)};
    robotSPI.writeBytes(initData);
    return true;
}

bool SPRK::teleopInit() {
    static const uint8_t initData[16] = {identToByte(COMMAND_IDENT::ROBOT_ENABLE)};
    robotSPI.writeBytes(initData);
    return true;
}

void SPRK::disabledInit() {
    static const uint8_t initData[16] = {identToByte(COMMAND_IDENT::ROBOT_DISABLE)};
    robotSPI.writeBytes(initData);
}

void SPRK::loop() {
    // Send heartbeat to MCU
    static const uint8_t heartbeatDataDisabled[16] = {identToByte(COMMAND_IDENT::MASTER_HEARTBEAT_DISABLE)};
    static const uint8_t heartbeatDataEnabled[16] = {identToByte(COMMAND_IDENT::MASTER_HEARTBEAT_ENABLED)};

    static uint64_t lastHeartbeatTime = 0;
    uint64_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::system_clock::now().time_since_epoch())
                               .count();
    if (currentTime - lastHeartbeatTime >= 800) {
        lastHeartbeatTime = currentTime;
        robotSPI.writeBytes(getCurrentState() == RobotState::DISABLED ? heartbeatDataDisabled : heartbeatDataEnabled);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

void SPRK::addJoystickButtons() {
    Trigger::create(joystick->buttonEvent(JoystickButton::START))
        .onTrue([&telem = this->telemetry]() {
            telem.log("Enabling verbose logging.", LogLevel::INFO);
            telem.setGlobalVerbose(true);
        });

    Trigger::create(joystick->buttonEvent(JoystickButton::LEFTSHOULDER))
        .onTrue([&pinch = this->pinchers]() {
            pinch->log("Setting pinchers to 0 degrees.", LogLevel::VERBOSE);
            pinch->setAngle(0);
        })
        .onFalse([&pinch = this->pinchers]() {
            pinch->log("Setting pinchers to 90 degrees.", LogLevel::VERBOSE);
            pinch->setAngle(180);
        });

    Trigger::create(joystick->buttonEvent(JoystickButton::RIGHTSHOULDER))
        .onTrue([&spi = this->robotSPI]() {
            static const uint8_t data[16] = {identToByte(COMMAND_IDENT::TEST_ONE)};
            spi.writeBytes(data);
        })
        .onFalse([&spi = this->robotSPI]() {
            static const uint8_t data[16] = {identToByte(COMMAND_IDENT::TEST_ZERO)};
            spi.writeBytes(data);
        });

    // Trigger::create(joystick->buttonEvent(JoystickButton::LEFTSHOULDER))
    //     .onTrue([&arm = this->arm]() {
    //         arm->log("Moving turret CW.", LogLevel::VERBOSE);
    //         arm->moveTurret(StepperDirection::CW);
    //     })
    //     .onFalse([&arm = this->arm]() {
    //         arm->log("Stopping turret.", LogLevel::VERBOSE);
    //         arm->moveTurret(StepperDirection::STOP);
    //     });
    // Trigger::create(joystick->buttonEvent(JoystickButton::RIGHTSHOULDER))
    //     .onTrue([&arm = this->arm]() {
    //         arm->log("Moving turret CCW.", LogLevel::VERBOSE);
    //         arm->moveTurret(StepperDirection::CCW);
    //     })
    //     .onFalse([&arm = this->arm]() {
    //         arm->log("Stopping turret.", LogLevel::VERBOSE);
    //         arm->moveTurret(StepperDirection::STOP);
    //     });

    // Trigger::create(joystick->buttonEvent(JoystickButton::DPADUP))
    //     .onTrue([&arm = this->arm]() {
    //         arm->log("Moving arm CW.", LogLevel::VERBOSE);
    //         arm->moveArm(StepperDirection::CW);
    //     })
    //     .onFalse([&arm = this->arm]() {
    //         arm->log("Stopping arm.", LogLevel::VERBOSE);
    //         arm->moveArm(StepperDirection::STOP);
    //     });

    // Trigger::create(joystick->buttonEvent(JoystickButton::DPADDOWN))
    //     .onTrue([&arm = this->arm]() {
    //         arm->log("Moving arm CCW.", LogLevel::VERBOSE);
    //         arm->moveArm(StepperDirection::CCW);
    //     })
    //     .onFalse([&arm = this->arm]() {
    //         arm->log("Stopping arm.", LogLevel::VERBOSE);
    //         arm->moveArm(StepperDirection::STOP);
    //     });

    // Trigger::create(joystick->buttonEvent(JoystickButton::DPADRIGHT))
    //     .onTrue([&arm = this->arm]() {
    //         arm->log("Moving wrist CW.", LogLevel::VERBOSE);
    //         arm->moveWrist(StepperDirection::CW);
    //     })
    //     .onFalse([&arm = this->arm]() {
    //         arm->log("Stopping wrist.", LogLevel::VERBOSE);
    //         arm->moveWrist(StepperDirection::STOP);
    //     });

    // Trigger::create(joystick->buttonEvent(JoystickButton::DPADLEFT))
    //     .onTrue([&arm = this->arm]() {
    //         arm->log("Moving wrist CCW.", LogLevel::VERBOSE);
    //         arm->moveWrist(StepperDirection::CCW);
    //     })
    //     .onFalse([&arm = this->arm]() {
    //         arm->log("Stopping wrist.", LogLevel::VERBOSE);
    //         arm->moveWrist(StepperDirection::STOP);
    //     });
}
