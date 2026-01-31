#pragma once

#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "Constants.hpp"

class RobotCamera {
    public:
        RobotCamera();
        ~RobotCamera();

        void start();
        void stop();

        bool isStarted() const {
            return _isStarted;
        }

    private:
        bool _isStarted {false};

        pid_t cameraProcess;
        std::string cameraCommand;
};
