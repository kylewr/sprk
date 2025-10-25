#pragma once

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "RobotEnums.hpp"

#define _TERM_RESET "\033[0m"
#define _TERM_RED "\033[91m"
#define _TERM_GREEN "\033[92m"
#define _TERM_YELLOW "\033[93m"
#define _TERM_BLUE "\033[94m"
#define _SOCKET_NEWLINE "%NL%"
#define _SOCKET_RED "%RED%"
#define _SOCKET_GREEN "%GREEN%"
#define _SOCKET_YELLOW "%YELLOW%"

class RobotTelemetry {
    public:
        RobotTelemetry(std::ostream* stream = &std::cout) : consoleStream(stream) {};
        ~RobotTelemetry() = default;

        void setConsoleStream(std::ostream* stream) {
            consoleStream = stream;
        }

        void setSocketSupplier(std::function<void(const std::string&)> supplier) {
            socketSupplier = supplier;
        }

        void setGlobalVerbose(bool enabled) {
            globalVerbose = enabled;
        }

        bool getGlobalVerbose() const {
            return globalVerbose;
        }

        void log(const std::string& message, LogLevel level, bool onlyConsole = false);

        void logRobotState(RobotState state);

    private:
        static std::stringstream getLogStream() {
            auto now       = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            auto milliseconds =
                std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) %
                1000;
            ss << '[' << std::put_time(std::localtime(&in_time_t), "%m/%d %X") << '.'
               << std::setfill('0') << std::setw(3) << milliseconds.count() << "] ";
            return ss;
        }

        bool validStream() const {
            return consoleStream != nullptr;
        }

        bool validSocket() const {
            return socketSupplier != nullptr;
        }

        std::ostream* consoleStream;
        std::function<void(const std::string&)> socketSupplier;

        bool globalVerbose {false};
};

class SubsystemTelemetry {
    public:
        SubsystemTelemetry(const std::string& name) : subsystemName(name) {}
        ~SubsystemTelemetry() = default;

        void provideRobotTelemetry(RobotTelemetry* telemetry);

        void onProvision(std::function<void()> callback) {
            onProvisionCallback = callback;
        }

        bool log(const std::string& message, LogLevel level);

        void setVerbose(bool enabled) {
            verbose = enabled;
        }

        bool getVerbose() const {
            return verbose;
        }

    private:
        const std::string subsystemName;

        RobotTelemetry* robotTelemetry;
        std::function<void()> onProvisionCallback;
        bool verbose {false};
};
