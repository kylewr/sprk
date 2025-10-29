#pragma once

#include <functional>
#include <string>
#include <thread>

class SerialInterface {
    public:
        SerialInterface(const std::string& portName, unsigned int baudRate);
        virtual ~SerialInterface();

        virtual bool openPort();
        virtual void closePort();

        bool getIsOpen() const {
            return isOpen;
        }

        void onReceive(std::function<void(const std::string&)> handler) {
            receiveHandler = handler;
        }

        virtual bool writeData(const std::string& data);

        void startMultiCommand() {
            isMultiCommandMode = true;
            multiCommand = ";";
        }

    protected:
        const std::string portName;
        const unsigned int baudRate;
        std::function<void(const std::string&)> receiveHandler;

        int fd;
        bool isOpen {false};

        bool isMultiCommandMode {false};
        std::string multiCommand{};

    private:
        bool configurePort();
};
