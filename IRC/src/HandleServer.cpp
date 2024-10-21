#include "Server.hpp"

void Server::handleServerCommand(int clientSocket, const std::string &arguments) {
 
    if (!_clients[clientSocket].isOper() && !_clients[clientSocket].isAdmin()) {
        sendMessageToClient(clientSocket, 482, ":You're not operator or admin.\r\n");
        return;
    }

    std::vector<std::string> serverNames = split(arguments, ' ');

    if (serverNames.size() > 1) {
        sendMessageToClient(clientSocket, 461, "SERVER", ":Too many parameters\r\n");
        return;
    }

    if (serverNames.empty() || serverNames[0].empty()) {
        sendMessageToClient(clientSocket, 461, "SERVER", ":No server name provided\r\n");
        return;
    }

    std::string serverName = serverNames[0];
    trim(serverName);
    setServerName(serverName);
}
