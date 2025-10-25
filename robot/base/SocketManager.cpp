#include "SocketManager.hpp"

#include <thread>
#include <sstream>
#include <chrono>
#include <iostream>

SocketManager::SocketManager(SocketManagerArgs * args)
{
    if (args != nullptr) {
        setArgs(args);
    }
}

SocketManager::~SocketManager()
{
    delete socketArgs;
}

bool SocketManager::initializeSocket()
{
    if (socketArgs == nullptr) {
        return false;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(socketArgs->portNumber);
    serverAddr.sin_addr.s_addr = inet_addr(socketArgs->ipAddress.c_str());
    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(sockfd, 5);

    initialized = true;

    socketListenerThread();

    return true;
}

void SocketManager::closeSocket()
{
    initialized = false;
}

bool SocketManager::sendMessage(const std::string &message)
{
    if (!initialized) {
        return false;
    }

    ssize_t bytesSent = send(connection, message.c_str(), message.size(), 0);
    if (bytesSent >= 0) {
        return true;
    }

    return false;
}

void SocketManager::socketListenerThread()
{
    std::thread([this]() {
        while (this->initialized) {
            std::cout << "\033[90mSocket listener thread started, waiting for connections.\033[0m\n";
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
                        buffer[bytesRead] = '\0';  // Null-terminate the string
                        std::string in = std::string(buffer);
                        
                        this->socketArgs->incomingMessageHandler(in);
                    } else if (bytesRead == 0) {
                        this->socketArgs->socketMessageHandler("Client disconnected.", LogLevel::WARN);
                        // close(this->sockfd);
                        this->connection = -1;
                        break;
                    } else {
                        this->socketArgs->socketMessageHandler("Error reading from socket.", LogLevel::ERROR);
                        // close(this->sockfd);
                        this->connection = -1;
                        break;
                    }
                }
                
            } else {
                socketArgs->socketMessageHandler("Failed to accept incoming connection.", LogLevel::ERROR);
            }
        }
        if (this->connection != -1) {
            close(this->sockfd);
            this->connection = -1;
        }
    }).detach();
}
