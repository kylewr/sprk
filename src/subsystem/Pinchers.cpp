#include "Pinchers.hpp"

#include "SPIMappings.hpp"

Pinchers::Pinchers(RobotSPI* robotSPI) : Subsystem("PINCHERS"), robotSPI(robotSPI) {}

void Pinchers::setAngle(uint16_t angle) {
    uint16_t clampedAngle = std::max(MIN_ANGLE, std::min(MAX_ANGLE, angle));
    sendAngleBytes((clampedAngle >> 8) & 0xFF, clampedAngle & 0xFF);
}

void Pinchers::sendAngleBytes(uint8_t highByte, uint8_t lowByte) {
    uint8_t data[16] = {
        commandToByte(COMMAND_IDENT::CONTROL_PINCHERS),
        highByte,
        lowByte
    };
    robotSPI->writeBytes(data);
}
