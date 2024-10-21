#include "Server.hpp"

Server::Server(const std::string &port, const std::string &password)
    : _port(port), _password(password), _serverSocket(-1), _adminPassword("admin123") {}

Server::~Server() {
}

const std::string Server::getServerName() const{ return _serverName;}
void Server::setServerName(const std::string &serverName){ _serverName = serverName;}

void Server::start() {
    initServerSocket();

    std::cout << "Server socket initialized successfully." << std::endl;

    while (g_server == false) {
        int pollResult = poll(_pollFds.data(), _pollFds.size(), -1);
        if (pollResult < 0) {
            std::cerr << "Error: poll() failed: " << strerror(errno) << std::endl;
            break;
        }

        for (size_t i = 0; i < _pollFds.size(); ++i) {
            if (_pollFds[i].revents & POLLIN) {
                if (_pollFds[i].fd == _serverSocket) {
                    std::cout << "New connection detected." << std::endl;
                    handleNewConnection();
                } else {
                    std::cout << "Handling client message for socket " << _pollFds[i].fd << std::endl;
                    handleClientMessage(_pollFds[i].fd);
                }
            }
        }
    }
    stop();
}









