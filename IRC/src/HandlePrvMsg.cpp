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
            sendMessageToClient(clientSocket, "Please specify a recipient and a message.\n");
            return;
        }
    } else {
        sendMessageToClient(clientSocket, "Please specify a recipient and a message.\n");
        return;
    }

    if (sendPrivateMessage(_clients[clientSocket].getNickName(), receiver, msg)) {
        return;
    }

    std::string errorMessage = "Error: User " + receiver + " not found.\n";
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
            std::string fullMessage = "Private from " + sender + ": " + message + "\n";
            sendMessageToClient(it->second.getSocket(), fullMessage);
            return 1;
        }
    }
    return 0; 
}
