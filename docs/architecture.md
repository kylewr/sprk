# SPRK Robot Architecture

## Class Diagram

```mermaid
classDiagram
    class RobotBase {
        <<abstract>>
        +Telemetry telemetry
        +SocketManager socketManager
        +InternalJoystick* internalController
        +changeState(RobotState)
        +addSubsystem(Subsystem*)
        +run()
        #loop()
    }

    class SPRK {
        -Arm* arm
        -Drivetrain* drivetrain
        -Pinchers* pinchers
        -SerialInterface* serialInterface
        +loop()
        +addJoystickButtons()
    }

    class Subsystem {
        <<abstract>>
        #SubsystemTelemetry telemetry
        +getTelemetry()
        +changedState(RobotState)
    }

    class Arm {
        -VirtualStepperManager* stepperManager
        -GPIOServo* servo
        +moveTurret(StepperDirection)
        +moveArm(StepperDirection)
        +moveWrist(StepperDirection)
    }

    class Drivetrain {
        -HBridgeMotor[4] motors
        +driveRobotCentric(x, y, rotation)
        +stop()
    }

    class Pinchers {
        -GPIOServo* servo
        +open()
        +close()
    }

    class SerialInterface {
        -int fd
        -string portName
        -unsigned int baudRate
        +openPort()
        +writeData(string)
        +onReceive(callback)
    }

    class SocketManager {
        -int socketFd
        -string ipAddress
        -uint16_t port
        +initializeSocket()
        +sendMessage(string)
        +hasConnection()
    }

    class Telemetry {
        +log(message, LogLevel)
        +logRobotState(RobotState)
    }

    class TriggerManager {
        <<singleton>>
        -vector~shared_ptr~Trigger~~
        +add(Trigger)
        +process()
    }

    class Trigger {
        -function condition
        -function onTrueAction
        -function onFalseAction
        +create(condition)$
        +onTrue(action)
        +onFalse(action)
    }

    class HBridgeMotor {
        -MotorPins pinConfig
        -unsigned int pwmFreq
        -PigpioClient& client
        +initialize()
        +setSpeed(double)
        +stop()
    }

    class PigpioClient {
        <<singleton>>
        -int pigpioHandle
        +ensureInitialized()
        +handle()
    }

    class VirtualStepperManager {
        -SerialInterface* serial
        -VirtualStepper[3] steppers
        +getStepperByPort(int)
        +processIncomingMessage(string)
    }

    class GPIOServo {
        -unsigned int pin
        -PigpioClient& client
        +setPosition(int)
    }

    %% Inheritance
    RobotBase <|-- SPRK
    Subsystem <|-- Arm
    Subsystem <|-- Drivetrain
    Subsystem <|-- Pinchers

    %% Composition
    SPRK *-- Arm
    SPRK *-- Drivetrain
    SPRK *-- Pinchers
    SPRK *-- SerialInterface
    RobotBase *-- SocketManager
    RobotBase *-- Telemetry
    Arm *-- VirtualStepperManager
    Arm *-- GPIOServo
    Drivetrain *-- HBridgeMotor
    Pinchers *-- GPIOServo
    VirtualStepperManager *-- SerialInterface

    %% Dependency
    SPRK ..> TriggerManager
    TriggerManager o-- Trigger
    HBridgeMotor ..> PigpioClient
    GPIOServo ..> PigpioClient
```

## Component Architecture

```mermaid
graph TB
    subgraph "Robot Core"
        SPRK[SPRK Robot]
        RobotBase[RobotBase]
        TriggerMgr[TriggerManager]
    end

    subgraph "Subsystems"
        Arm[Arm Subsystem]
        Drive[Drivetrain Subsystem]
        Pinch[Pinchers Subsystem]
    end

    subgraph "Communication"
        Socket[SocketManager]
        Serial[SerialInterface]
    end

    subgraph "Hardware Control"
        Pigpio[PigpioClient]
        Motors[HBridgeMotor x4]
        Servos[GPIOServo x2]
        Steppers[VirtualStepperManager]
    end

    subgraph "External"
        Controller[Game Controller]
        Arduino[Arduino/Serial Device]
        NetworkClient[Network Client]
        GPIO[Raspberry Pi GPIO]
    end

    SPRK --> RobotBase
    SPRK --> Arm
    SPRK --> Drive
    SPRK --> Pinch
    SPRK --> TriggerMgr
    
    RobotBase --> Socket
    SPRK --> Serial
    
    Arm --> Steppers
    Arm --> Servos
    Drive --> Motors
    Pinch --> Servos
    
    Steppers --> Serial
    Motors --> Pigpio
    Servos --> Pigpio
    
    Socket --> NetworkClient
    Serial --> Arduino
    Pigpio --> GPIO
    Controller --> RobotBase
```

## Directory Structure

```
src/
├── main.cpp                    # Entry point
├── SPRK.{hpp,cpp}             # Main robot class
├── Constants.hpp               # Configuration constants
│
├── base/                       # Core framework
│   ├── RobotBase.{hpp,cpp}    # Base robot class
│   ├── Subsystem.hpp           # Subsystem interface
│   ├── Telemetry.{hpp,cpp}    # Logging system
│   ├── SocketManager.{hpp,cpp} # Network communication
│   ├── SerialInterface.{hpp,cpp} # Serial communication
│   ├── Joystick.hpp            # Controller input
│   ├── Trigger.{hpp,cpp}       # Event-based triggers
│   ├── RobotEnums.hpp          # State enums
│   ├── RobotHelpers.hpp        # Utility functions
│   │
│   ├── actuation/              # Hardware control
│   │   ├── PigpioClient.{hpp,cpp}
│   │   ├── HBridgeMotor.{hpp,cpp}
│   │   ├── GPIOServo.{hpp,cpp}
│   │   ├── VirtualStepper.{hpp,cpp}
│   │   ├── VirtualStepperManager.{hpp,cpp}
│   │   └── StepperConstants.hpp
│   │
│   ├── command/                # Command pattern
│   │   ├── BaseCommand.hpp
│   │   └── AutonomousCommand.hpp
│   │
│   └── simulation/             # Simulation support
│       └── SerialSimulation.hpp
│
└── subsystem/                  # Robot subsystems
    ├── Arm.{hpp,cpp}          # Arm control
    ├── Drivetrain.hpp         # Drive base
    └── Pinchers.hpp           # Gripper mechanism
```

## Data Flow

```mermaid
sequenceDiagram
    participant Controller
    participant SocketManager
    participant RobotBase
    participant TriggerManager
    participant Subsystems
    participant Hardware

    Controller->>SocketManager: Send joystick data
    SocketManager->>RobotBase: Parse & update controller state
    
    loop Main Loop
        RobotBase->>TriggerManager: process()
        TriggerManager->>TriggerManager: Evaluate conditions
        TriggerManager->>Subsystems: Execute actions
        Subsystems->>Hardware: Control motors/servos
        Hardware-->>Subsystems: Feedback (if any)
        RobotBase->>RobotBase: loop()
    end
    
    RobotBase->>SocketManager: Send telemetry
    SocketManager->>Controller: Update display
```
