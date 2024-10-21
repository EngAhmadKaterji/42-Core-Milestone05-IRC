#include "Server.hpp"

void Server::stop() {
    if (_serverSocket != -1) {
        close(_serverSocket);
        _serverSocket = -1;
    }

    for (size_t i = _pollFds.size(); i > 0; --i) {
        if (_pollFds[i - 1].fd != -1) {
            close(_pollFds[i - 1].fd);
            _pollFds[i - 1].fd = -1;
            
            std::map<int, Client>::iterator it = _clients.find(_pollFds[i - 1].fd);
            if (it != _clients.end()) {
                _clients.erase(it);
            }
        }
    }

    _pollFds.clear();
}
