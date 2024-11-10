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
        << " " << _clients[clientSocket].getNickName() << " " << message;
    std::string fullMessage = oss.str() + "\r\n";
    
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
        << " " << target << " :" << message ;
    std::string fullMessage = oss.str() + "\r\n";
    
    int bytesSent = send(clientSocket, fullMessage.c_str(), fullMessage.size(), 0);
    if (bytesSent < 0) {
        std::cerr << "Error sending message to client " << clientSocket << std::endl;
    } else if (bytesSent < static_cast<int>(fullMessage.size())) {
        std::cerr << "Partial message sent to client " << clientSocket << ": " << bytesSent << " bytes" << std::endl;
    }
}

void Server::sendMessageToChannel(const std::string &channel, const std::string &message, int senderSocket, int mode) {
    std::string sendmsg;

    if (mode == 1) {
        sendmsg = message + "\r\n";
    } else if (mode == 2) {
        sendmsg = ":" + _clients[senderSocket].getNickName() + " MODE " + channel + " " + message + "\r\n";
    } else if (mode == 0) {
        sendmsg = ":" + _clients[senderSocket].getNickName() + "!"
                + _clients[senderSocket].getUserName() + "@"
                + _clients[senderSocket].getHostName() + " PRIVMSG "
                + channel + " :" + message + "\r\n";
    }

    for (std::set<int>::iterator it = _channels[channel].getClients().begin(); it != _channels[channel].getClients().end(); ++it) {
        int clientSocket = *it;
        // Include sender in mode messages
        if (clientSocket != senderSocket || mode != 0) {
            int bytesSent = send(clientSocket, sendmsg.c_str(), sendmsg.length(), 0);
            if (bytesSent < 0) {
                std::cerr << "Error sending message to client " << clientSocket << std::endl;
            } else if (bytesSent < static_cast<int>(sendmsg.size())) {
                std::cerr << "Partial message sent to client " << clientSocket << ": " << bytesSent << " bytes" << std::endl;
            }
        }
    }
}

