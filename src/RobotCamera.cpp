#include "RobotCamera.hpp"

RobotCamera::RobotCamera() : cameraProcess(-1) {
    cameraCommand = std::string("mjpg_streamer -i \"input_uvc.so -d /dev/video0 -r ") +
                    Constants::CameraConstants::RES + " -f " +
                    std::to_string(Constants::CameraConstants::FPS) + " -ex " +
                    Constants::CameraConstants::EXPOSURE + "\" -o \"output_http.so\"";
}

RobotCamera::~RobotCamera() {
    stop();
}

void RobotCamera::start() {
    // Check if camera is already running
    if (cameraProcess != -1) {
        int status;
        pid_t result = waitpid(cameraProcess, &status, WNOHANG);
        if (result == 0) {
            std::cout << "Camera is already running." << std::endl;
            return;
        }
    }

    // Fork and execute the camera command
    cameraProcess = fork();
    if (cameraProcess == 0) {
        // Child process
        execl("/bin/sh", "sh", "-c", cameraCommand.c_str(), nullptr);
        // If execl fails
        std::cerr << "Failed to start camera process." << std::endl;
        exit(1);
    } else if (cameraProcess < 0) {
        std::cerr << "Failed to fork camera process." << std::endl;
        cameraProcess = -1;
    } else {
        std::cout << "Camera started." << std::endl;
    }
    _isStarted = true;
}

void RobotCamera::stop() {
    if (cameraProcess != -1) {
        // Send SIGTERM to the process
        kill(cameraProcess, SIGTERM);

        // Wait for process to terminate with timeout
        int status;
        int waitCount = 0;
        while (waitCount < 50) { // 5 seconds timeout (50 * 100ms)
            pid_t result = waitpid(cameraProcess, &status, WNOHANG);
            if (result != 0) {
                break;
            }
            usleep(100000); // 100ms
            waitCount++;
        }

        // Kill process if it didn't terminate
        if (waitCount >= 50) {
            kill(cameraProcess, SIGKILL);
            waitpid(cameraProcess, &status, 0);
            std::cout << "Camera process killed due to timeout." << std::endl;
        }

        // Additional cleanup: pkill mjpg_streamer
        system("pkill -f mjpg_streamer");

        cameraProcess = -1;
        std::cout << "Camera stopped." << std::endl;
    } else {
        std::cout << "No camera process to stop." << std::endl;
    }
    _isStarted = false;
}
