#pragma once

#include <string>
#include <unordered_map>

enum class RobotState {
    DISABLED,
    AUTONOMOUS,
    TELEOP,
};

inline std::string robotStateToString(RobotState state) {
    static const std::unordered_map<RobotState, std::string> stateToString {
        {RobotState::DISABLED, "DISABLED"},
        {RobotState::AUTONOMOUS, "AUTONOMOUS"},
        {RobotState::TELEOP, "TELEOP"},
    };
    return stateToString.at(state);
}

enum class LogLevel {
    VERBOSE,
    INFO,
    SUCCESS,
    WARN,
    ERROR,
};
