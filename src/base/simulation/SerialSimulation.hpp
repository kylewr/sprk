#pragma once

#include <functional>
#include <string>

#include "base/SerialInterface.hpp"

class SerialSimulation : public SerialInterface {
    public:
        using SerialInterface::SerialInterface;
        ~SerialSimulation() = default;

        bool openPort() override {
            isOpen = true;
            return true;
        };
        void closePort() override {
            isOpen = false;
        };

        bool writeData(const std::string& data) override {
            if (!isOpen) {
                return false;
            }

            if (receiveHandler) {
                receiveHandler(data);
            }
            return true;
        };
};
