#include "SerialInterface.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cerrno>
#include <iostream>

#include "base/actuation/StepperConstants.hpp"

SerialInterface::SerialInterface(const std::string& portName, unsigned int baudRate)
    : portName(portName), baudRate(baudRate) {}

SerialInterface::~SerialInterface() {
    closePort();
}

bool SerialInterface::openPort() {
    fd = open(portName.c_str(), O_RDWR | O_NOCTTY);

    if (fd < 0) {
        return false;
    }

    if (!configurePort()) {
        close(fd);
        return false;
    }

    // Clear O_NONBLOCK to allow VTIME to work properly
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);

    isOpen = true;

    if (receiveHandler != nullptr) {
        std::thread([this]() {
            std::string lineBuffer;
            char buffer[256];
            while (this->isOpen && fd >= 0) {
                ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
                if (bytesRead > 0) {
                    lineBuffer.append(buffer, bytesRead);
                    
                    // Process complete lines (delimited by \n)
                    size_t pos;
                    while ((pos = lineBuffer.find('\n')) != std::string::npos) {
                        std::string line = lineBuffer.substr(0, pos);
                        lineBuffer.erase(0, pos + 1);
                        
                        // Remove trailing \r if present
                        if (!line.empty() && line.back() == '\r') {
                            line.pop_back();
                        }
                        
                        if (!line.empty()) {
                            receiveHandler(line);
                        }
                    }
                } else if (bytesRead < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                    // Real error occurred
                    break;
                }
                // bytesRead == 0 is timeout, just continue
            }
        }).detach();
    }
    return true;
}

void SerialInterface::closePort() {
    if (fd >= 0) {
        isOpen = false;
        close(fd);
        fd = -1;
    }
}

bool SerialInterface::writeData(const std::string& data) {
    if (fd < 0) {
        return false;
    }
    if (isMultiCommandMode) {
        if (data[0] != '\n') {
            multiCommand += data;
            if (multiCommand.back() == '!') {
                multiCommand.pop_back(); // remove last '!'
            }
            multiCommand += '.';
            return true;
        }
        isMultiCommandMode = false;
        multiCommand.pop_back(); // remove last '.'
        multiCommand += SERIAL_END_BYTE;
        ssize_t bytesWritten = write(fd, multiCommand.c_str(), multiCommand.size());
        return bytesWritten == static_cast<ssize_t>(multiCommand.size());
    }

    ssize_t bytesWritten = write(fd, data.c_str(), data.size());
    return bytesWritten == static_cast<ssize_t>(data.size());
}

bool SerialInterface::configurePort() {
    struct termios options;

    if (tcgetattr(fd, &options) != 0) {
        return false;
    }

    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);

    options.c_cflag
        = (options.c_cflag & ~CSIZE) | CS8; // 8-bit characters
    options.c_iflag &= ~IGNBRK; // disable break processing
    options.c_lflag = 0; // no signaling chars, no echo, no
                     // canonical processing
    options.c_oflag = 0; // no remapping, no delays
    options.c_cc[VMIN] = 0; // read doesn't block
    options.c_cc[VTIME] = 5; // 0.5 seconds read timeout

    options.c_iflag &= ~(IXON | IXOFF
                     | IXANY); // shut off xon/xoff ctrl

    options.c_cflag
        |= (CLOCAL | CREAD); // ignore modem controls,
                             // enable reading
    options.c_cflag &= ~(PARENB | PARODD); // shut off parity
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        return false;
    }

    return true;
}
