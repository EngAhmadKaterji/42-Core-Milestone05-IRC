#include "Server.hpp"

void Server::sendUpdatedNamesList(const std::string &channel) {
    std::string userList = ":" + _serverName + " 353 " + "server" + " = " + channel + " :";
    for (std::set<int>::iterator it = _channels[channel].getClients().begin(); it != _channels[channel].getClients().end(); ++it) {
        if (_channels[channel].isOperator(*it)) {
            userList += "@" + _clients[*it].getNickName() + " ";
        } else {
            userList += _clients[*it].getNickName() + " ";
        }
    }

    userList = userList.substr(0, userList.size() - 1) + "\r\n";

    for (std::set<int>::iterator it = _channels[channel].getClients().begin(); it != _channels[channel].getClients().end(); ++it) {
        sendMessageToClient(*it, userList);
    }

    std::string endOfListMessage = ":" + _serverName + " 366 " + "server" + " " + channel + " :End of /NAMES list\r\n";
    for (std::set<int>::iterator it = _channels[channel].getClients().begin(); it != _channels[channel].getClients().end(); ++it) {
        sendMessageToClient(*it, endOfListMessage);
    }
}
