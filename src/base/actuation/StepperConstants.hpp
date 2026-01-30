#pragma once

#define SERIAL_END_BYTE "!"

class VirtualStepperConstants {
    public:
        static constexpr const char* ENABLE_CMD = ",0" SERIAL_END_BYTE;
        static constexpr const char* DISABLE_CMD = ",1" SERIAL_END_BYTE;
};

enum class StepperDirection {
    CW,
    CCW,
    STOP,
};

namespace StepperHelpers {
    inline StepperDirection invertDirection(StepperDirection dir) {
        switch (dir) {
            case StepperDirection::CW:
                return StepperDirection::CCW;
            case StepperDirection::CCW:
                return StepperDirection::CW;
            case StepperDirection::STOP:
            default:
                return StepperDirection::STOP;
        }
    }
}
