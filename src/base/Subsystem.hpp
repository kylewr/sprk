#pragma once

#include <string>

#include "Telemetry.hpp"

class Subsystem {
    public:
        Subsystem(const std::string& name) : telemetry(name) {
            telemetry.onProvision([this]() {
                this->telemetry.log("Telemetry initialized.", LogLevel::INFO);
            });
        };
        ~Subsystem() {
            telemetry.log("Subsystem destroyed.", LogLevel::VERBOSE);
        };

        SubsystemTelemetry* getTelemetry() {
            return &telemetry;
        }
    
    protected:
        SubsystemTelemetry telemetry;

};