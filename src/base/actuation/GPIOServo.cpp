#include "GPIOServo.hpp"

GPIOServo::GPIOServo(unsigned int port, const ServoArgs* args) : port(port), currentAngle(0.0) {
    this->args = new ServoArgs();
    this->args->minAngle = args->minAngle;
    this->args->maxAngle = args->maxAngle;

}
