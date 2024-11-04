#include "Server.hpp"

int Server::findClientSocketByNick(const std::string &nickname) {
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second.getNickName() == nickname) {
            return it->first;
        }
    }
    return -1;
}
