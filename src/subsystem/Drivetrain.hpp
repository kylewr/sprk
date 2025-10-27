#pragma once

#include "base/Subsystem.hpp"

class Drivetrain : public Subsystem {
    public:
        Drivetrain() : Subsystem("DRIVETRAIN") {}
        ~Drivetrain() = default;
};
