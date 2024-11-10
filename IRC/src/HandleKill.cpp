#include "Server.hpp"

void Server::handleKillCommand(int clientSocket, const std::string &target) {
   std::string message;
    if (!_clients[clientSocket].isAdmin() && !_clients[clientSocket].isOper()) {
        message = ":" + getServerName() + " 481 " + _clients[clientSocket].getNickName() + " :Permission Denied - You're not an IRC operator\r\n";
        sendMessageToClient(clientSocket, message);
        std::cerr << "Error: Client " << clientSocket << " does not have permission to kill channels or clients.\n";
        return;
    }

    std::map<std::string, Channel>::iterator channelIt = _channels.find(target);
    if (channelIt != _channels.end()) {
        std::set<int>::iterator clientIt;
        for (clientIt = channelIt->second.getClients().begin(); clientIt != channelIt->second.getClients().end(); ++clientIt) {
            int clientFd = *clientIt;
            _clients[clientFd].removeChannel(target);
            message = ":" + getServerName() + " KILL " + target + " :Channel has been removed\r\n";
            sendMessageToClient(clientFd, message);
            std::cout << "Client " << clientFd << " removed from channel: " << target << std::endl;
        }
        
        _channels.erase(channelIt);
        std::cout << "Channel " << target << " has been removed.\n";
        return;
    }

    bool clientFound = false;
    std::map<int, Client>::iterator clientIt;
    for (clientIt = _clients.begin(); clientIt != _clients.end(); ++clientIt) {
        if (clientIt->second.getNickName() == target) {
            int targetSocket = clientIt->first;
            message = ":" + getServerName() + " ERROR :Closing link (" + _clients[targetSocket].getNickName() + "@" + _clients[targetSocket].getHostName() + ") [Killed]\r\n";
            sendMessageToClient(targetSocket, message);
            removeClient(targetSocket);
            clientFound = true;
            std::cout << "Client " << target << " has been disconnected from the server.\n";
            break;
        }
    }

    if (!clientFound) {
        message = ":" + getServerName() + " 401 " + _clients[clientSocket].getNickName() + " " + target + " :No such nick/channel\r\n";
        sendMessageToClient(clientSocket, message);
        std::cerr << "Error: User or channel " << target << " not found.\n";
    }
}
