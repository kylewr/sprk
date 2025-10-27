#pragma once

#include "Constants.hpp"
#include "base/Subsystem.hpp"

class Pinchers : public Subsystem {
    public:
        Pinchers() : Subsystem("PINCHERS") {}
        ~Pinchers() = default;
};
