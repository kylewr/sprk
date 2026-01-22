#pragma once

#include "spi.h"

class RobotSPI {
    public:
        RobotSPI(uint8_t deviceNum, uint8_t bitExchangeSize, uint32_t speed, uint8_t mode,
                 bool useLock)
            : deviceNumber(deviceNum), bitExchangeSize(bitExchangeSize), speed(speed), mode(mode),
              useLock(useLock) {}
        ~RobotSPI();

        void initialize();

        bool isOpen();
        void close();
        int writeAndRead(uint8_t* txData, uint8_t* rxData, uint16_t length, bool leaveCsLow);
        int writeBlockRepeat(uint8_t* txBlock, uint16_t txBlockLen, uint32_t repeats,
                             bool leaveCsLow);

        int writeBytes(uint8_t* txData);
        int writeBytes(const uint8_t* txData);

    private:
        SPI_HANDLE spiHandle;

        uint8_t deviceNumber;
        uint8_t bitExchangeSize;
        uint32_t speed;
        uint8_t mode;
        bool useLock;
};
