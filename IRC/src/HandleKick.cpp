#include "Server.hpp"

void Server::handleKickCommand(int clientSocket, const std::string &message) {
    std::string sendmsg;
    std::size_t colonPos = message.find(":");
    if (colonPos == std::string::npos) {
        sendmsg = "Invalid KICK command format. Use: KICK <channel> <user> :<reason>\n";
        sendMessageToClient(clientSocket, sendmsg);
        return;
    }

    std::string arguments = message.substr(0, colonPos);
    std::istringstream iss(arguments);
    std::string channel, target;
    iss >> channel >> target;

    if (channel.empty() || target.empty()) {
        sendmsg =  "Invalid KICK command format. Use: KICK <channel> <user> :<reason>\n";
        sendMessageToClient(clientSocket, sendmsg);
        return;
    }

    std::string reason = message.substr(colonPos + 1);

if (!_channels[channel].isOperator(clientSocket)) {
    sendmsg = ":" + getServerName() + " 482 " + _clients[clientSocket].getNickName() + " " + channel + " :You're not a channel operator\r\n";
    sendMessageToClient(clientSocket, sendmsg);
    std::cerr << "Error: Client " << clientSocket << " does not have permission to kick users from channels.\n";
    return;
}

std::map<std::string, Channel>::iterator channelIt = _channels.find(channel);
if (channelIt == _channels.end() || channelIt->second.getClients().find(clientSocket) == channelIt->second.getClients().end()) {
    sendmsg = ":" + getServerName() + " 403 " + _clients[clientSocket].getNickName() + " " + channel + " :No such channel\r\n";
    sendMessageToClient(clientSocket, sendmsg);
    return;
}

int targetSocket = -1;
std::map<int, Client>::iterator it = _clients.begin();
for (; it != _clients.end(); ++it) {
    if (it->second.getNickName() == target) {
        targetSocket = it->first;
        break;
    }
}

if (targetSocket == -1) {
    sendmsg = ":" + getServerName() + " 401 " + _clients[clientSocket].getNickName() + " " + target + " :No such nick/channel\r\n";
    sendMessageToClient(clientSocket, sendmsg);
    return;
}

channelIt->second.removeClient(targetSocket);
sendmsg = "You have been kicked from " + channel + ": " + reason + "\r\n";
sendMessageToClient(targetSocket, sendmsg);
sendmsg = target + " has been kicked from the channel. Reason: " + reason + "\r\n";
sendMessageToChannel(channel, sendmsg, clientSocket);

if (channelIt->second.isEmpty()) {
    _channels.erase(channelIt);
}
}