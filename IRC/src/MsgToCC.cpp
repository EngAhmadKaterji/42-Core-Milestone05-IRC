#include "Server.hpp"

void Server::sendMessageToClient(int clientSocket, const std::string &message) {
    std::string msg = message + "\r\n";
    int bytesSent = send(clientSocket, msg.c_str(), msg.length(), 0);
    
    if (bytesSent < 0) {
        std::cerr << "Error sending message to client " << clientSocket << std::endl;
    } else if (bytesSent < static_cast<int>(msg.size())) {
        std::cerr << "Partial message sent to client " << clientSocket << ": " << bytesSent << " bytes" << std::endl;
    }
}

void Server::sendMessageToClient(int clientSocket, int replyCode, const std::string &message) {
    std::ostringstream oss;
    oss << ":" << _serverName << " " << replyCode
        << " " << _clients[clientSocket].getNickName() << " " << message << "\r\n";
    std::string fullMessage = oss.str();
    
    int bytesSent = send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0);
    if (bytesSent < 0) {
        std::cerr << "Error sending message to client " << clientSocket << std::endl;
    } else if (bytesSent < static_cast<int>(fullMessage.size())) {
        std::cerr << "Partial message sent to client " << clientSocket << ": " << bytesSent << " bytes" << std::endl;
    }
}

void Server::sendMessageToClient(int clientSocket, int replyCode, const std::string &target, const std::string &message) {
    std::ostringstream oss;
    oss << ":" << _serverName << " " << replyCode << " " << _clients[clientSocket].getNickName()
        << " " << target << " :" << message << "\r\n";
    std::string fullMessage = oss.str();
    
    int bytesSent = send(clientSocket, fullMessage.c_str(), fullMessage.size(), 0);
    if (bytesSent < 0) {
        std::cerr << "Error sending message to client " << clientSocket << std::endl;
    } else if (bytesSent < static_cast<int>(fullMessage.size())) {
        std::cerr << "Partial message sent to client " << clientSocket << ": " << bytesSent << " bytes" << std::endl;
    }
}

void Server::sendMessageToChannel(const std::string &channel, const std::string &message, int senderSocket) {
    std::string sendmsg = "PRIVMSG " + channel + " :" + message + "\r\n";

    std::set<int>::iterator it;
    for (it = _channels[channel].getClients().begin(); it != _channels[channel].getClients().end(); ++it) {
        int clientSocket = *it;
        if (clientSocket != senderSocket) {
            int bytesSent = send(clientSocket, sendmsg.c_str(), sendmsg.length(), 0);
            if (bytesSent < 0) {
                std::cerr << "Error sending message to client " << clientSocket << std::endl;
            } else if (bytesSent < static_cast<int>(sendmsg.size())) {
                std::cerr << "Partial message sent to client " << clientSocket << ": " << bytesSent << " bytes" << std::endl;
            }
        }
    }
}
