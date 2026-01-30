#pragma once

class ServoArgs {
    public:
        ServoArgs() = default;
        ~ServoArgs() = default;

        double minAngle = 0.0;
        double maxAngle = 180.0;
};

class GPIOServo {
    public:
        GPIOServo(unsigned int port, const ServoArgs* args);
        ~GPIOServo() = default;

        void setAngle(double angle);
        double getAngle() const;

    private:
        ServoArgs* args;

        unsigned int port;
        double currentAngle;
};