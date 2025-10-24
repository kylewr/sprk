#include "SocketManager.hpp"

#include <thread>
#include <sstream>
#include <chrono>

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

    isInitialized = true;

    socketListenerThread();

    return true;
}

void SocketManager::closeSocket()
{
}

bool SocketManager::sendMessage(const std::string &message)
{
    return false;
}

void SocketManager::socketListenerThread()
{
    int clientSock = accept(sockfd, nullptr, nullptr);
    if (clientSock >= 0) {
        std::thread([clientSock, this]() {
            while (this->isInitialized) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                char buffer[1024];
                int bytesRead = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0';  // Null-terminate the string
                    std::string in = std::string(buffer);
                    this->socketArgs->socketMessageHandler("Received message: " + in, LogLevel::INFO);
                    this->socketArgs->incomingMessageHandler(in);
                } else if (bytesRead == 0) {
                    this->socketArgs->socketMessageHandler("Client disconnected.", LogLevel::WARN);
                    close(clientSock);
                } else {
                    this->socketArgs->socketMessageHandler("Error reading from socket.", LogLevel::ERROR);
                    close(clientSock);
                }
            }
        }).detach();
    } else {
        socketArgs->socketMessageHandler("Failed to accept incoming connection.", LogLevel::ERROR);
    }
}
