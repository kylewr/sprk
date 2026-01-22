#pragma once

#include <cstdint>
#include <unordered_map>

enum class COMMAND_IDENT : uint8_t {
    // reserve 0x00-0x09 for no operation
    NO_OP = 0x00,
    MASTER_HEARTBEAT_DISABLE = 0x01,
    MASTER_HEARTBEAT_ENABLED = 0x02,

    // reserve 0x0A-0x1F for system commands
    SYSTEM_RESET = 0x10,

    // reserve 0x20-0x2F for LED statuses.
    LEDS_ONBOARD_CTRL = 0x20,
    LEDS_RSL_CTRL = 0x21,

    // reserve 0x30-0x3F for robot states
    ROBOT_DISABLE = 0x30,
    ROBOT_ENABLE = 0x31,

    // reserve 0xA0-0xEF for user-defined commands
    CONTROL_DRIVETRAIN = 0xA0,
    CONTROL_PINCHERS = 0xC0,

    // reserve 0xF0-0xFF for test commands
    TEST_ZERO = 0xF0,
    TEST_ONE = 0xF1,

};

static const std::unordered_map<uint8_t, COMMAND_IDENT> byteToIdent {
    {0x00, COMMAND_IDENT::NO_OP},
    {0x01, COMMAND_IDENT::MASTER_HEARTBEAT_DISABLE},
    {0x02, COMMAND_IDENT::MASTER_HEARTBEAT_ENABLED},
    {0x10, COMMAND_IDENT::SYSTEM_RESET},
    {0x20, COMMAND_IDENT::LEDS_ONBOARD_CTRL},
    {0x21, COMMAND_IDENT::LEDS_RSL_CTRL},
    {0x30, COMMAND_IDENT::ROBOT_DISABLE},
    {0x31, COMMAND_IDENT::ROBOT_ENABLE},
    {0xA0, COMMAND_IDENT::CONTROL_DRIVETRAIN},
    {0xC0, COMMAND_IDENT::CONTROL_PINCHERS},
    {0xF0, COMMAND_IDENT::TEST_ZERO},
    {0xF1, COMMAND_IDENT::TEST_ONE},
};

static inline uint8_t identToByte(COMMAND_IDENT ident) {
    return static_cast<uint8_t>(ident);
}
