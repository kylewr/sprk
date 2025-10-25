#include "Telemetry.hpp"

void RobotTelemetry::log(const std::string& message, LogLevel level, bool onlyConsole) {
    if (level == LogLevel::VERBOSE && !globalVerbose) {
        return;
    }

    std::stringstream logStream = getLogStream();
    logStream << message;

    std::string consoleHeader {_TERM_RESET};
    std::string socketHeader {_SOCKET_NEWLINE};
    switch (level) {
        case LogLevel::SUCCESS:
            consoleHeader = _TERM_GREEN;
            socketHeader += _SOCKET_GREEN;
            break;
        case LogLevel::WARN:
            consoleHeader = _TERM_YELLOW;
            socketHeader += _SOCKET_YELLOW;
            break;
        case LogLevel::ERROR:
            consoleHeader = _TERM_RED;
            socketHeader += _SOCKET_RED;
            break;
        case LogLevel::SPECIAL_GRAY:
            consoleHeader = _TERM_GRAY;
            break;
        default:
            break;
    }

    if (validStream()) {
        *consoleStream << consoleHeader << logStream.str() << _TERM_RESET << '\n';
    }
    if (validSocket() && !onlyConsole) {
        socketSupplier(socketHeader + logStream.str());
    }
}

void RobotTelemetry::logRobotState(RobotState state) {
    if (!validStream()) {
        return;
    }

    *consoleStream << _TERM_BLUE << getLogStream().str() << "Robot changed state: " << _TERM_RESET
                   << robotStateToString(state) << '\n';

    if (validSocket()) {
        socketSupplier("[STATE] " + robotStateOverSocket(state));
    }
}

void SubsystemTelemetry::provideRobotTelemetry(RobotTelemetry* telemetry) {
    robotTelemetry = telemetry;

    if (onProvisionCallback != nullptr) {
        onProvisionCallback();
        onProvisionCallback = nullptr;
    }
}

bool SubsystemTelemetry::log(const std::string& message, LogLevel level) {
    if (robotTelemetry != nullptr) {
        if (level == LogLevel::VERBOSE && !robotTelemetry->getGlobalVerbose() && !getVerbose()) {
            return true;
        } else if (level == LogLevel::VERBOSE) {
            level = LogLevel::INFO;
        }
        robotTelemetry->log("(" + subsystemName + ") " + message, level);
        return true;
    }
    return false;
}
