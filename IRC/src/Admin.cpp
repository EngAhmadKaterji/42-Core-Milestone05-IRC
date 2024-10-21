#include "Server.hpp"

void Server::handleAdminCommand(int clientSocket, const std::string &password)
{
    if( password == _adminPassword){
        _clients[clientSocket].setAdmin(true);
        std::string message = "You are now an admin.\n";
        sendMessageToClient(clientSocket, message);
        std::cout << "Client " << clientSocket << " failed admin authentication." << std::endl;
    }
     else {
        std::string message = "Error: Invalid admin password.\n";
        sendMessageToClient(clientSocket, message);
        std::cout << "Client " << clientSocket << " failed admin authentication." << std::endl;
    }
}