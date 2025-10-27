#pragma once

#include <termios.h>

namespace Constants {
    class IOMap {
        public:
            static constexpr unsigned int SERVO_PORT = 8;

            static constexpr unsigned int TURRET_ID = 0;
            static constexpr unsigned int ARM_ID = 1;
            static constexpr unsigned int WRIST_ID = 2;

            static constexpr const char* SERIAL_PORT = "/dev/ttyACM0";
            static constexpr unsigned int BAUD_RATE = B115200;
    };
}