#include "SocketManager.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

SocketManager::SocketManager(SocketManagerArgs* args) {
    if (args != nullptr) {
        setArgs(args);
    }
}

SocketManager::~SocketManager() {
    if (sockfd != -1) {
        close(sockfd);
    }

    if (connection != -1) {
        close(connection);
    }

    delete socketArgs;
}

bool SocketManager::initializeSocket() {
    if (socketArgs == nullptr) {
        return false;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return false;
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(socketArgs->portNumber);
    serverAddr.sin_addr.s_addr = inet_addr(socketArgs->ipAddress.c_str());
    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(sockfd, 5);

    initialized = true;

    createListenerThread();

    return true;
}

void SocketManager::closeSocket() {
    initialized = false;
}

bool SocketManager::sendMessage(const std::string& message) {
    if (!initialized) {
        return false;
    }

    ssize_t bytesSent = send(connection, message.c_str(), message.size(), 0);
    if (bytesSent >= 0) {
        return true;
    }

    return false;
}

void SocketManager::createListenerThread() {
    std::thread([this]() {
        while (this->initialized) {
            char ipStr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(this->serverAddr.sin_addr), ipStr, INET_ADDRSTRLEN);
            this->socketArgs->socketMessageHandler(
                "Waiting for incoming connection on: " + std::string(ipStr) + ":" +
                    std::to_string(ntohs(this->serverAddr.sin_port)),
                LogLevel::SPECIAL_GRAY);
            int clientSock = accept(sockfd, nullptr, nullptr);
            this->connection = clientSock;
            if (clientSock >= 0) {
                this->socketArgs->socketMessageHandler("Client connected.", LogLevel::SUCCESS);

                if (this->onConnectCallback != nullptr) {
                    this->onConnectCallback();
                }

                while (this->initialized) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    char buffer[1024];
                    int bytesRead = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
                    if (bytesRead > 0) {
                        buffer[bytesRead] = '\0'; // Null-terminate the string
                        std::string in = std::string(buffer);

                        this->socketArgs->incomingMessageHandler(in);
                    } else if (bytesRead == 0) {
                        this->socketArgs->socketMessageHandler("Client disconnected.",
                                                               LogLevel::WARN);
                        // close(this->sockfd);
                        this->connection = -1;
                        break;
                    } else {
                        this->socketArgs->socketMessageHandler("Error reading from socket.",
                                                               LogLevel::ERROR);
                        // close(this->sockfd);
                        this->connection = -1;
                        break;
                    }
                }

            } else {
                socketArgs->socketMessageHandler("Failed to accept incoming connection.",
                                                 LogLevel::ERROR);
            }
        }
        if (this->connection != -1) {
            close(this->sockfd);
            this->connection = -1;
        }
    }).detach();
}
