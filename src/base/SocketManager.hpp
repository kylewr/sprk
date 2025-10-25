#pragma once

#include <cstdint>
#include <functional>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include "RobotEnums.hpp"

class SocketManagerArgs {
    public:
        SocketManagerArgs() = default;

        std::string ipAddress {"127.0.0.1"};
        uint16_t portNumber {8080};
        std::function<void(const std::string&, LogLevel)> socketMessageHandler {nullptr};
        std::function<void(const std::string&)> incomingMessageHandler {nullptr};
};

class SocketManager {
    public:
        SocketManager(SocketManagerArgs* args = nullptr);
        ~SocketManager();

        bool setArgs(SocketManagerArgs* args) {
            if (initialized) {
                return false;
            }
            socketArgs = args;
            return true;
        }

        bool initializeSocket();
        void closeSocket();
        bool sendMessage(const std::string& message);

        bool isInitialized() const {
            return initialized;
        }

        bool hasConnection() const {
            return connection != -1;
        }

        void onConnect(std::function<void()> callback) {
            onConnectCallback = callback;
        }

    private:
        SocketManagerArgs* socketArgs;

        int sockfd;
        int connection {-1};
        struct sockaddr_in serverAddr;
        bool initialized {false};

        std::function<void()> onConnectCallback {nullptr};

        void createListenerThread();
};
