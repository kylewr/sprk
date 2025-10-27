#pragma once

#include <string>
#include <unordered_map>
#include <vector>

enum class JoystickButton {
    NONE = -1,

    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    LEFTSHOULDER = 4,
    RIGHTSHOULDER = 5,
    BACK = 6,
    START = 7,
    LEFTTHUMB = 8,
    RIGHTTHUMB = 9,
    DPADUP = 10,
    DPADDOWN = 11,
    DPADLEFT = 12,
    DPADRIGHT = 13,

    _NONE = 14,
    _A = 15,
    _B = 16,
    _X = 17,
    _Y = 18,
    _LEFTSHOULDER = 19,
    _RIGHTSHOULDER = 20,
    _BACK = 21,
    _START = 22,
    _LEFTTHUMB = 23,
    _RIGHTTHUMB = 24,
    _DPADUP = 25,
    _DPADDOWN = 26,
    _DPADLEFT = 27,
    _DPADRIGHT = 28
};

namespace JoystickButtonUtil {
    inline const std::unordered_map<JoystickButton, std::string> NAMES = {
        {JoystickButton::NONE, "NONE"},
        {JoystickButton::A, "A"},
        {JoystickButton::B, "B"},
        {JoystickButton::X, "X"},
        {JoystickButton::Y, "Y"},
        {JoystickButton::LEFTSHOULDER, "LEFTSHOULDER"},
        {JoystickButton::RIGHTSHOULDER, "RIGHTSHOULDER"},
        {JoystickButton::BACK, "BACK"},
        {JoystickButton::START, "START"},
        {JoystickButton::LEFTTHUMB, "LEFTTHUMB"},
        {JoystickButton::RIGHTTHUMB, "RIGHTTHUMB"},
        {JoystickButton::DPADUP, "DPADUP"},
        {JoystickButton::DPADDOWN, "DPADDOWN"},
        {JoystickButton::DPADLEFT, "DPADLEFT"},
        {JoystickButton::DPADRIGHT, "DPADRIGHT"},
        {JoystickButton::_A, "-A"},
        {JoystickButton::_B, "-B"},
        {JoystickButton::_X, "-X"},
        {JoystickButton::_Y, "-Y"},
        {JoystickButton::_LEFTSHOULDER, "-LEFTSHOULDER"},
        {JoystickButton::_RIGHTSHOULDER, "-RIGHTSHOULDER"},
        {JoystickButton::_BACK, "-BACK"},
        {JoystickButton::_START, "-START"},
        {JoystickButton::_LEFTTHUMB, "-LEFTTHUMB"},
        {JoystickButton::_RIGHTTHUMB, "-RIGHTTHUMB"},
        {JoystickButton::_DPADUP, "-DPADUP"},
        {JoystickButton::_DPADDOWN, "-DPADDOWN"},
        {JoystickButton::_DPADLEFT, "-DPADLEFT"},
        {JoystickButton::_DPADRIGHT, "-DPADRIGHT"}};

    inline const std::unordered_map<JoystickButton, JoystickButton> RELEASE_MAP = {
        {JoystickButton::NONE, JoystickButton::_NONE},
        {JoystickButton::A, JoystickButton::_A},
        {JoystickButton::B, JoystickButton::_B},
        {JoystickButton::X, JoystickButton::_X},
        {JoystickButton::Y, JoystickButton::_Y},
        {JoystickButton::LEFTSHOULDER, JoystickButton::_LEFTSHOULDER},
        {JoystickButton::RIGHTSHOULDER, JoystickButton::_RIGHTSHOULDER},
        {JoystickButton::BACK, JoystickButton::_BACK},
        {JoystickButton::START, JoystickButton::_START},
        {JoystickButton::LEFTTHUMB, JoystickButton::_LEFTTHUMB},
        {JoystickButton::RIGHTTHUMB, JoystickButton::_RIGHTTHUMB},
        {JoystickButton::DPADUP, JoystickButton::_DPADUP},
        {JoystickButton::DPADDOWN, JoystickButton::_DPADDOWN},
        {JoystickButton::DPADLEFT, JoystickButton::_DPADLEFT},
        {JoystickButton::DPADRIGHT, JoystickButton::_DPADRIGHT}};

    inline JoystickButton getReleasedValue(JoystickButton button) {
        auto it = RELEASE_MAP.find(button);
        return (it != RELEASE_MAP.end()) ? it->second : button;
    }

    inline std::string getStr(JoystickButton button) {
        return NAMES.at(button);
    }

    inline JoystickButton fromString(const std::string& name) {
        for (const auto& pair : NAMES) {
            if (pair.second == name) {
                return pair.first;
            }
        }
        return JoystickButton::NONE;
    }

    inline std::vector<std::string> getAll() {
        std::vector<std::string> result;
        for (const auto& pair : NAMES) {
            result.push_back(pair.second);
        }
        return result;
    }
} // namespace JoystickButtonUtil

enum class JoystickAxis {
    LEFT_X = 0,
    LEFT_Y = 1,
    RIGHT_X = 2,
    RIGHT_Y = 3,
    LEFT_TRIGGER = 4,
    RIGHT_TRIGGER = 5
};

namespace JoystickAxisUtil {
    inline std::unordered_map<JoystickAxis, float> convertFromList(const std::vector<int>& axis) {
        return {{JoystickAxis::LEFT_X, static_cast<float>(axis[0])},
                {JoystickAxis::LEFT_Y, static_cast<float>(axis[1])},
                {JoystickAxis::RIGHT_X, static_cast<float>(axis[2])},
                {JoystickAxis::RIGHT_Y, static_cast<float>(axis[3])},
                {JoystickAxis::LEFT_TRIGGER, static_cast<float>(axis[4])},
                {JoystickAxis::RIGHT_TRIGGER, static_cast<float>(axis[5])}};
    }
} // namespace JoystickAxisUtil
