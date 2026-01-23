#include "RobotSPI.hpp"
#include <stdexcept>

RobotSPI::~RobotSPI() {
    close();
}

void RobotSPI::initialize() {
    spiHandle = SpiOpenPort(deviceNumber, bitExchangeSize, speed, mode, useLock);
    if (spiHandle == nullptr) {
        throw std::runtime_error("Failed to open SPI port.");
    }
}

bool RobotSPI::isOpen() {
    return spiHandle != nullptr;
}

void RobotSPI::close() {
    if (spiHandle != nullptr) {
        SpiClosePort(spiHandle);
        spiHandle = nullptr;
    }
}

int RobotSPI::writeAndRead(uint8_t* txData, uint8_t* rxData, uint16_t length, bool leaveCsLow) {
    if (spiHandle == nullptr) {
        return -1;
    }
    return SpiWriteAndRead(spiHandle, txData, rxData, length, leaveCsLow);
}

int RobotSPI::writeAndRead(const uint8_t* txData, uint8_t* rxData) {
    if (spiHandle == nullptr) {
        return -1;
    }
    return SpiWriteAndRead(spiHandle, const_cast<uint8_t*>(txData), rxData, sizeof(txData), false);
}

int RobotSPI::writeBlockRepeat(uint8_t* txBlock, uint16_t txBlockLen, uint32_t repeats,
                               bool leaveCsLow) {
    if (spiHandle == nullptr) {
        return -1;
    }
    return SpiWriteBlockRepeat(spiHandle, txBlock, txBlockLen, repeats, leaveCsLow);
}

int RobotSPI::writeBytes(uint8_t* txData) {
    if (spiHandle == nullptr) {
        return -1;
    }
    return writeAndRead(txData, nullptr, sizeof(txData), false);
}

int RobotSPI::writeBytes(const uint8_t* txData) {
    if (spiHandle == nullptr) {
        return -1;
    }
    return writeAndRead(const_cast<uint8_t*>(txData), nullptr, sizeof(txData), false);
}
