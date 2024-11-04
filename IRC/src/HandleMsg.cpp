#include "Server.hpp"
#include <sstream>
#include <map>
#include <string>

typedef void (Server::*CommandHandler)(int, const std::string&);

void Server::handleClientMessage(int clientSocket) {
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead <= 0 || (_clients[clientSocket].getState() == HANDSHAKE
        && _clients[clientSocket].getNoOfTries() == 2)) {
        sendMessageToClient(clientSocket, "You are disconnected.\n");
        if (_clients[clientSocket].getNoOfTries() == 2) {
            std::cout << "Client " << clientSocket << " after 3 attempts to connect." << std::endl;
        } else {
            std::cout << "Client " << clientSocket << " disconnected." << std::endl;
        }
        removeClient(clientSocket);
        return;
    }

    buffer[bytesRead] = '\0';
    _clients[clientSocket].setMessage(buffer);
    std::string clientBuffer = _clients[clientSocket].getMessage();

    size_t pos;
    while ((pos = clientBuffer.find("\n")) != std::string::npos) {
        std::string line = clientBuffer.substr(0, pos);
        clientBuffer.erase(0, pos + 1);                 
        std::istringstream iss(line);
        std::string command, arguments;

        iss >> command;
        command = toUpperCase(command);
        std::getline(iss, arguments);
        trim(arguments);

        if (_clients[clientSocket].getState() == HANDSHAKE && command == "PASS") {
            handlePassCommand(clientSocket, arguments);
            continue;
        }

        if (_clients[clientSocket].isInviteState()) {
            trim(command);
            handleInviteeCommand(clientSocket, command);
            continue;
        }

        if (_clients[clientSocket].getState() == LOGIN) {
            std::map<std::string, CommandHandler> commandHandlers;
            commandHandlers["JOIN"] = &Server::handleJoinCommand;
            commandHandlers["INVITE"] = &Server::handleInviteCommand;
            commandHandlers["NICK"] = &Server::handleNickCommand;
            commandHandlers["ADMIN"] = &Server::handleAdminCommand;
            commandHandlers["PING"] = &Server::handlePingCommand;
            commandHandlers["LIST"] = &Server::handleListCommand;
            commandHandlers["USER"] = &Server::handleUserCommand;
            commandHandlers["PART"] = &Server::handlePartCommand;
            commandHandlers["KICK"] = &Server::handleKickCommand;
            commandHandlers["PRIVMSG"] = &Server::handlePrvMsg;
            commandHandlers["KILL"] = &Server::handleKillCommand;
            commandHandlers["MODE"] = &Server::handleModeCommand;
            commandHandlers["TOPIC"] = &Server::handleTopicCommand;
            commandHandlers["WHO"] = &Server::handleWhoCommand;
            commandHandlers["SERVER"] = &Server::handleServerCommand;

            std::map<std::string, CommandHandler>::iterator it = commandHandlers.find(command);
            if (it != commandHandlers.end()) {
                (this->*(it->second))(clientSocket, arguments);
                continue;
            }
        }

        for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
            const std::string &channel = it->first;
            const std::set<int> &clients = it->second.getClients();
            if (clients.find(clientSocket) != clients.end()) {
                sendMessageToChannel(channel, line, clientSocket);
            }
        }

        std::cout << "Received message: " << line << std::endl;
    }

    if (clientBuffer.empty()) {
        _clients[clientSocket].eraseMessage();
    }
}
