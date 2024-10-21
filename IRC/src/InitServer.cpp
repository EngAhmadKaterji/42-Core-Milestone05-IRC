#include "Server.hpp"

void Server::initServerSocket() {
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0) {
        throw std::runtime_error("Error: Failed to create socket");
    }

    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(_serverSocket);
        throw std::runtime_error("Error: Failed to set socket options");
    }

    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(std::atoi(_port.c_str()));

    if (bind(_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(_serverSocket);
        throw std::runtime_error("Error: Failed to bind socket");
    }

    if (listen(_serverSocket, SOMAXCONN) < 0) {
        close(_serverSocket);
        throw std::runtime_error("Error: Failed to listen on socket");
    }

    fcntl(_serverSocket, F_SETFL, O_NONBLOCK);

    struct pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN;
    _pollFds.push_back(serverPollFd);

    std::cout << "Server started on port " << _port << std::endl;
}
