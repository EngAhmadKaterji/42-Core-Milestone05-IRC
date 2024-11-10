#include "Server.hpp"

void Server::handleJoinCommand(int clientSocket, const std::string &arguments) {
    std::string channel, password;
    std::istringstream iss(arguments);
    std::vector<std::string> userDetails;
    std::string detail;
    std::string message;

    while (iss >> detail) {
        userDetails.push_back(detail);
    }
    
    std::string nickname = _clients[clientSocket].getNickName();
    std::string serverName = getServerName();
    

    if (userDetails.empty()) {
        message = ":" + serverName + " 461 " + nickname + " JOIN :Not enough parameters\r\n";
        sendMessageToClient(clientSocket, message);
        return;
    }

    channel = userDetails[0];

    if (userDetails.size() > 1) {
        password = userDetails[1];
    }

    if (_channels.find(channel) == _channels.end()) {
        _channels[channel] = Channel(channel);
        if (!password.empty()) {
            _channels[channel].setPassword(password);
        } 
    } else if (_channels[channel].isInviteOnly() &&
               _channels[channel].getInvitedClients().find(clientSocket) == _channels[channel].getInvitedClients().end()) {
        message = ":" + serverName + " 473 " + nickname + " " + channel + " :Cannot join channel (+i)\r\n";
        sendMessageToClient(clientSocket, message);
        return;
    } else {
        if (!_channels[channel].getPassword().empty() && _channels[channel].getPassword() != password) {
            message = ":" + serverName + " 475 " + nickname + " " + channel + " :Cannot join channel (+k)\r\n";
            sendMessageToClient(clientSocket, message);
            return;
        }
    }

    int type = _channels[channel].addClient(clientSocket);
    if (type == 2) { 
        message = ":" + serverName + " 471 " + nickname + " " + channel + " :Cannot join channel (+l)\r\n";
        sendMessageToClient(clientSocket, message);
        return;
    } else if (type == 3) {
        _channels[channel].removeInvitedClient(clientSocket);
    }

    if (_channels[channel].clientCount() == 1) {
        _channels[channel].addOperator(clientSocket);
    }
    _clients[clientSocket].setChannel(channel);
    message = ":" + nickname + "!" + nickname + "@" + serverName + " JOIN :" + channel;
    sendMessageToClient(clientSocket, message);
    sendMessageToChannel(channel, message, clientSocket, 0);
    sendUpdatedNamesList(channel);

    std::cout << "Client " << clientSocket << " joined channel: " << channel << std::endl;
}
