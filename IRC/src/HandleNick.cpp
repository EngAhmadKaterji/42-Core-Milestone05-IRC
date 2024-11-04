#include "Server.hpp"

void Server::handleNickCommand(int clientSocket, const std::string &argument) {
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        int otherClientSocket = it->first; 
        if (otherClientSocket != clientSocket && argument == it->second.getNickName()) {
            sendMessageToClient(clientSocket, 433, "*", argument + " :Nickname is already in use"+".\r\n");
            return ;
        }
    }

    _clients[clientSocket].setNickName(argument);
    sendMessageToClient(clientSocket,": 001 " + argument + ":Welcome to the IRC Network " + argument + ".\r\n"); 
    std::cout << "Client " << clientSocket << " set their nickname to: " 
              << _clients[clientSocket].getNickName() << std::endl;
}
