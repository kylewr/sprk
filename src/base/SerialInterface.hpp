#pragma once

#include <string>
#include <thread>
#include <functional>

class SerialInterface {
    public:
        SerialInterface(const std::string& portName, unsigned int baudRate);
        ~SerialInterface();

        bool openPort();
        void closePort();

        bool getIsOpen() const {
            return isOpen;
        }

        void onReceive(std::function<void(const std::string&)> handler) {
            receiveHandler = handler;
        }
    
        bool writeData(const std::string& data);

    private:
        const std::string portName;
        const unsigned int baudRate;
        std::function<void(const std::string&)> receiveHandler;

        int fd;
        bool isOpen {false};

        bool configurePort();
};