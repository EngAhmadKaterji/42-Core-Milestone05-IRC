#include "Server.hpp"

void Server::handlePartCommand(int clientSocket, const std::string &arguments) {
    std::string channelNames, message;
    size_t colonPos = arguments.find(':');
    if (colonPos != std::string::npos) {
        channelNames = arguments.substr(0, colonPos);
        message = arguments.substr(colonPos + 1);
    } else {
        channelNames = arguments;
    }

    std::vector<std::string> channelList = split(channelNames, ',');
    for (std::vector<std::string>::iterator it = channelList.begin(); it != channelList.end(); ++it) {
        std::string channelName = *it;
        trim(channelName);
        std::map<std::string, Channel>::iterator channelIt = _channels.find(channelName);

        if (channelIt == _channels.end()) {
            sendMessageToClient(clientSocket, 403, channelName, ":No such channel");
            continue;
        }

        Channel& channel = channelIt->second; 
        if (!channel.isClient(clientSocket)) {
            sendMessageToClient(clientSocket, 442, channelName, ":You're not on that channel");
            continue;
        }

        std::string nickName = _clients[clientSocket].getNickName();
        std::string partMessage = ":" + nickName + " PART " + channelName;

        if (!message.empty()) {
            trim(message);
            partMessage += " :" + message;
        }

        sendMessageToChannel(channelName, partMessage, clientSocket);
        channel.removeClient(clientSocket);
        if (channel.getClients().size() == 0)
        _channels.erase(channel.getName());
        std::cout << "Client " << clientSocket << " has left " << channelName << "." << std::endl;
    }
}
