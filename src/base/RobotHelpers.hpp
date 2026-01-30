#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "RobotEnums.hpp"

#define ROBOT_INFO_PREFIX "[ROBOTINFO]"
#define ROBOT_INFO_AUTONS_TAG "[SIG][AUTONS]"
#define ROBOT_INFO_FLAGS_TAG "[SIG][FLAGS]"

class RobotHelpers {
    public:
        static std::string toCSV(std::vector<std::string> items) {
            std::string result;
            for (size_t i = 0; i < items.size(); ++i) {
                result += items[i];
                if (i < items.size() - 1) {
                    result += ",";
                }
            }
            return result;
        }

        static std::string toCSV(std::vector<RobotFlags> flags) {
            std::string result {","};
            for (size_t i = 0; i < flags.size(); ++i) {
                result += robotFlagToString(flags[i]);
                if (i < flags.size() - 1) {
                    result += ",";
                }
            }
            return result;
        }

        static std::vector<std::string> split(const std::string& str, char delimiter) {
            std::vector<std::string> tokens;
            std::stringstream ss(str);
            std::string token;

            while (std::getline(ss, token, delimiter)) {
                tokens.push_back(token);
            }

            return tokens;
        }
};

class RobotInfoArgs {
    public:
        std::string str() const {
            std::string result {ROBOT_INFO_PREFIX};

            result += message;
            result += ROBOT_INFO_AUTONS_TAG + RobotHelpers::toCSV(autons);
            result += ROBOT_INFO_FLAGS_TAG + RobotHelpers::toCSV(flags) + "\n";

            return result;
        }

        std::string message {};
        std::vector<std::string> autons {}; // TODO: convert to type
        std::vector<RobotFlags> flags {};
};
