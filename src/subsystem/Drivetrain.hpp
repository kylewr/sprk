#pragma once

#include <cstdint>
#include <thread>

#include "SPIMappings.hpp"
#include "base/Subsystem.hpp"

class Drivetrain : public Subsystem {
    public:
        Drivetrain(RobotSPI* spi) : Subsystem("DRIVETRAIN"), spi(spi) {}
        ~Drivetrain() = default;

        void robotCentric(uint8_t x, uint8_t y, uint8_t r) {
            feedMotor(0, y + r - x); // front left
            feedMotor(1, y - r + x); // front right
            feedMotor(2, y + r + x); // back left
            feedMotor(3, y - r - x); // back right
            sendCommands();
        }

        void feedMotor(unsigned int motorIndex, uint8_t speed) {
            speed += 127;
            if (speed > 255) {
                speed = 255;
            } else if (speed < 0) {
                speed = 0;
            }
            switch (motorIndex) {
                case 0:
                    flSpeed = speed;
                    break;
                case 1:
                    frSpeed = speed;
                    break;
                case 2:
                    blSpeed = speed;
                    break;
                case 3:
                    brSpeed = speed;
                    break;
            }
        }

        void sendCommands() {
            sendCommand(flSpeed, frSpeed, brSpeed, blSpeed);
        }

    private:
        void sendCommand(uint8_t fl, uint8_t fr, uint8_t br, uint8_t bl) const {
            uint8_t commandBytes[16] = {commandToByte(COMMAND_IDENT::CONTROL_DRIVETRAIN), fl, fr,
                                        br, bl};
            spi->writeBytes(commandBytes);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        RobotSPI* spi;

        uint8_t flSpeed {0};
        uint8_t frSpeed {0};
        uint8_t blSpeed {0};
        uint8_t brSpeed {0};
};
