#include "Server.hpp"

void Server::removeClient(int clientSocket) {
    close(clientSocket);
  
    _clients.erase(clientSocket);

    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        it->second.removeClient(clientSocket);
    }

    for (std::vector<pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); ++it) {
        if (it->fd == clientSocket) {
            _pollFds.erase(it);
            break;
        }
    }
}
