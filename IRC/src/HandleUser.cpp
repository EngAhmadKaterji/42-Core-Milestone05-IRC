#include "Server.hpp"

void Server::handleUserCommand(int clientSocket, const std::string &arguments) {
    std::string names, realname;

    if (arguments.find(':') != std::string::npos) {
        size_t colonPos = arguments.find(':');
        names = arguments.substr(0, colonPos);
        realname = arguments.substr(colonPos + 1);
    } else {
        names = arguments;
    }

    if (_clients[clientSocket].getNickName().empty()) {
        sendMessageToClient(clientSocket, 451, "Please set your nickname first using the NICK command.\n");
        return;
    }

    std::istringstream iss(names);
    std::vector<std::string> userDetails;
    std::string detail;

    while (iss >> detail) {
        userDetails.push_back(detail);
    }

    if (userDetails.size() < 2) {
        sendMessageToClient(clientSocket, 461, "Insufficient parameters for USER command. Username and hostname are required.\n");
        return;
    }

    std::string username = userDetails[0];
    std::string hostname = userDetails[1];

    if (username.empty()) {
        sendMessageToClient(clientSocket, 432, "Username cannot be empty.\n");
        return;
    }

    _clients[clientSocket].setUsername(username);
    _clients[clientSocket].setHostname(hostname);
    
    if (!realname.empty()) {
        _clients[clientSocket].setRealName(realname);
    }

    std::cout << "Client " << clientSocket 
              << " set their username to: " << _clients[clientSocket].getUsername() 
              << ", hostname to: " << _clients[clientSocket].getHostname() 
              << ", real name to: " << _clients[clientSocket].getRealName() << std::endl;

    sendMessageToClient(clientSocket, 001, "Welcome " + _clients[clientSocket].getNickName() + "!" +
        " Your username, hostname, and real name have been set successfully.\n");
}
