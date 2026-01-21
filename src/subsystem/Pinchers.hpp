#pragma once

#include "Constants.hpp"
#include "base/Subsystem.hpp"
#include "base/spi/RobotSPI.hpp"

class Pinchers : public Subsystem {
    public:
        Pinchers(RobotSPI* robotSPI);
        ~Pinchers() = default;

        void setAngle(uint16_t angle);

    private:
        void sendAngleBytes(uint8_t highByte, uint8_t lowByte);

        static constexpr uint16_t MAX_ANGLE = 270;
        static constexpr uint16_t MIN_ANGLE = 0;

        RobotSPI* robotSPI;
};
