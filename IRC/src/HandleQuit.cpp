#include "Server.hpp"

void Server::handleQuitCommand(int clientSocket, const std::string &quitMessage) {
    std::string nickname = _clients[clientSocket].getNickName();
    std::string serverName = getServerName();
    std::string message;
    std::string reason = quitMessage;

    if (!quitMessage.empty()) {
        reason = quitMessage.substr(1, quitMessage.size());
        trim(reason);
    }
    message = ":" + nickname + "!" + nickname + "@" + serverName + " QUIT :" + (quitMessage.empty() ? "Client exited" : reason) + "\r\n";

    std::vector<Channel> channels = _clients[clientSocket].getChannels();
    removeClient(clientSocket);

    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
        const std::string &channelName = it->getName(); 
        sendMessageToChannel(channelName, message, clientSocket, 1);
        sendUpdatedNamesList(channelName);
    }
}

