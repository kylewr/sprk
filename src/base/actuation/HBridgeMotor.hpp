#pragma once

class HBridgeMotorArgs {
    public:
        HBridgeMotorArgs() = default;
        ~HBridgeMotorArgs() = default;

        unsigned int pinHigh;
        unsigned int pinLow;
        bool inverted {false};
};

class PWMHBridgeMotor {
    public:
        PWMHBridgeMotor(unsigned int port, HBridgeMotorArgs* args);
        ~PWMHBridgeMotor() = default;

    private:
        unsigned int port;
        HBridgeMotorArgs* args;
};