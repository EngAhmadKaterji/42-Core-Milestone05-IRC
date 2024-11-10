#include "Server.hpp"

void Server::handlePrvMsg(int clientSocket, const std::string &arguments) {
    std::istringstream iss(arguments);
    std::string receiver, msg;

    size_t pos = arguments.find(":");
    if (pos != std::string::npos) {
        receiver = arguments.substr(0, pos);
        msg = arguments.substr(pos + 1);

        trim(receiver);
        trim(msg);
        
        if (receiver.empty() || msg.empty()) {
            sendMessageToClient(clientSocket, "Please specify a recipient and a message.");
            return;
        }
    } else {
        sendMessageToClient(clientSocket, "Please specify a recipient and a message.");
        return;
    }

    if (sendPrivateMessage(_clients[clientSocket].getNickName(), receiver, msg)) {
        return;
    }

    std::string errorMessage = "Error: User / Channel " + receiver + " not found.";
    sendMessageToClient(clientSocket, 401, errorMessage);
}

int Server::sendPrivateMessage(const std::string &sender, const std::string &receiver, const std::string &message) {
    std::map<int, Client>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it) {
        std::string clientName = it->second.getNickName();
        std::string targetReceiver = receiver;

        trim(clientName);
        trim(targetReceiver);

        if (clientName == targetReceiver) {
            std::string fullMessage = "Private from " + sender + ": " + message;
            sendMessageToClient(it->second.getSocket(), fullMessage);
            return 1;
        }
    }

    std::map<std::string , Channel>::iterator channelIt;
    for (channelIt = _channels.begin(); channelIt != _channels.end(); ++channelIt) {
        std::string channelName = channelIt->second.getName();
        std::string targetReceiver = receiver;

        trim(channelName);
        trim(targetReceiver);

        if (channelName == targetReceiver) {
            int senderSocket = findClientSocketByNick(sender);
        if (_channels[channelName].getClients().find(senderSocket) != _channels[channelName].getClients().end()) {
            std::string fullMessage = "Private from " + sender + ": " + message;
            sendMessageToChannel(targetReceiver, fullMessage, senderSocket,0);
            return 1;
        } else {
            std::string errorMsg = "You are not in this channel.";
            sendMessageToClient(senderSocket, 442, channelName , errorMsg);
            return 1;
        }
        }
    }

    return 0;
}
