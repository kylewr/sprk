#pragma once

#include <cstdint>
#include <thread>

#include "SPIMappings.hpp"
#include "base/Subsystem.hpp"

class Drivetrain : public Subsystem {
    public:
        Drivetrain(RobotSPI* spi) : Subsystem("DRIVETRAIN"), spi(spi) {}
        ~Drivetrain() = default;

        void robotCentric(int x, int y, int r) {
            feedMotor(0, y - r + x); // front left
            feedMotor(1, y + r - x); // front right
            feedMotor(2, y - r - x); // back left
            feedMotor(3, y + r + x); // back right

            // log("Drivetrain command - X: " + std::to_string(x) + " Y: " + std::to_string(y) +
            //         " R: " + std::to_string(r),
            //     LogLevel::VERBOSE);

            sendCommands();
        }

        void feedMotor(unsigned int motorIndex, int speed) {
            // speed is -100-100
            
            if (speed > 100) {
                speed = 100;
            } else if (speed < -100) {
                speed = -100;
            }

            speed = static_cast<uint8_t>((speed + 100) * 127 / 100); // map to 0-254
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

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        RobotSPI* spi;

        uint8_t flSpeed {0};
        uint8_t frSpeed {0};
        uint8_t blSpeed {0};
        uint8_t brSpeed {0};
};
