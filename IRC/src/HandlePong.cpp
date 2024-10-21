#include "Server.hpp"

void Server::handlePingCommand(int clientSocket, const std::string &arguments) {
    if (arguments.empty()) {
        sendMessageToClient(clientSocket, 409, ":No origin specified"); // 409: ERR_NOSUCHSERVICE
        return;
    }

    std::string pongResponse = "PONG " + arguments + "\n";
    sendMessageToClient(clientSocket, pongResponse);
    std::cout << "Responded to PING with: " << pongResponse << " for client " << clientSocket << std::endl;
}
