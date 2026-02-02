#pragma once

#include <termios.h>

#define SPI_BYTE_SIZE 16

namespace Constants {
    class SPRKConstants {
        public:
            static constexpr unsigned int PORT = 8008;
            static constexpr const char* IP_ADDRESS = "0.0.0.0";
            
            static constexpr const char* INFOARGS_MESSAGE = "SPRK Robot\nDeveloped by Kyle Rush";
    };

    class SPIConstants {
        public:
            static constexpr unsigned int CHANNEL = 0;
            static constexpr unsigned int CHIP_SELECT_PIN = 8;
            static constexpr unsigned int CLOCK_SPEED_HZ = 1000000;
            static constexpr unsigned int MODE = 0;
            static constexpr bool MSBFIRST = true;
    };

    class IOMap {
        public:
            static constexpr unsigned int SERVO_PORT = 8;

            static constexpr unsigned int TURRET_ID = 0;
            static constexpr unsigned int ARM_ID = 1;
            static constexpr unsigned int WRIST_ID = 2;

            static constexpr const char* SERIAL_PORT = "/dev/ttyACM0";
            static constexpr unsigned int BAUD_RATE = B115200;
    };

    class CameraConstants {
        public:
            static constexpr const char* RES = "1280x1024";
            static constexpr int FPS = 30;
            static constexpr const char* EXPOSURE = "50";
    };
}