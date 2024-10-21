#include "Server.hpp"

void Server::handleListCommand(int clientSocket, const std::string &arguments) {
    std::string clientNick = _clients[clientSocket].getNickName();
    std::string listResponse;

    listResponse = ":" + getServerName() + " 321 " + clientNick + " Channel :Users Name\r\n";
    sendMessageToClient(clientSocket, listResponse);

    bool hasFilter = !arguments.empty();
    bool channelsFound = false;

    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        const std::string &channelName = it->first;
        const Channel &channel = it->second;

        if (hasFilter && channelName.find(arguments) == std::string::npos) {
            continue;
        }

        listResponse = ":" + getServerName() + " 322 " + clientNick + " #" + channelName + " " + 
                       to_string(channel.clientCount()) + " :" + channel.getTopic() + "\r\n";
        sendMessageToClient(clientSocket, listResponse);
        channelsFound = true;
    }

    if (!channelsFound) {
        listResponse = ":" + getServerName() + " 322 " + clientNick + " * 0 :No channels match the criteria\r\n";
        sendMessageToClient(clientSocket, listResponse);
    }

    listResponse = ":" + getServerName() + " 323 " + clientNick + " :End of LIST\r\n";
    sendMessageToClient(clientSocket, listResponse);
}
