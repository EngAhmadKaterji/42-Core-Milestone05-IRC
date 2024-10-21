#include "Server.hpp"

void Server::sendMessageToClient(int clientSocket, const std::string &message){
    send(clientSocket, message.c_str(), message.length(), 0);
}

void Server::sendMessageToClient(int clientSocket, int replyCode, const std::string &message) {
    std::ostringstream oss;
    oss << ":" << _serverName << " " << replyCode
        << " " << _clients[clientSocket].getNickName() << " " << message;
    std::string fullMessage = oss.str();
    send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0);
}

void Server::sendMessageToClient(int clientSocket, int replyCode, const std::string &target, const std::string &message) {
    std::ostringstream oss;
    oss << ":" << _serverName << " " << replyCode << " " << _clients[clientSocket].getNickName()
        << " " << target << " :" << message << "\r\n";
    std::string fullMessage = oss.str();
    send(clientSocket, fullMessage.c_str(), fullMessage.size(), 0);
}

void Server::sendMessageToChannel(const std::string &channel, const std::string &message, int senderSocket) {
    for (std::set<int>::iterator it = _channels[channel].getClients().begin(); it != _channels[channel].getClients().end(); ++it) {
        int clientSocket = *it;
        if (clientSocket != senderSocket) {
            std::string sendmsg = message + "\r\n";
            send(clientSocket, sendmsg.c_str(), sendmsg.length(), 0);
        }
    }
}