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

// TODO: mega remove ts LMAOOOOOOO
inline std::string robotStateOverSocket(RobotState state) {
    static const std::unordered_map<RobotState, std::string> stateToSocketString {
        {RobotState::DISABLED, "DISABLE"},
        {RobotState::AUTONOMOUS, "AUTONOMOUS"},
        {RobotState::TELEOP, "TELEOP"},
    };
    return stateToSocketString.at(state);
}

enum class LogLevel {
    VERBOSE,
    INFO,
    SUCCESS,
    WARN,
    ERROR,
};
