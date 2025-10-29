#pragma once

#include <functional>
#include <string>

#include "base/SerialInterface.hpp"
#include "base/actuation/StepperConstants.hpp"

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
                if (receiveHandler) {
                    receiveHandler(multiCommand);
                }
                return true;
            }

            if (receiveHandler) {
                receiveHandler(data);
            }
            return true;
        };
};
