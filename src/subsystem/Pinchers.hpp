#pragma once

#include "Constants.hpp"
#include "base/Subsystem.hpp"

class Pinchers : public Subsystem {
    public:
        Pinchers() : Subsystem("PINCHERS") {
            telemetry.onProvision([this]() {
                this->telemetry.log("Pinchers subsystem initialized.", LogLevel::INFO);

                

            });
        }
        ~Pinchers() = default;

    private:
};
