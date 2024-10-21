#include "Server.hpp"

void Server::handleInviteOnlyMode(int clientSocket, const std::string &channel, int flag) {
    
    if (!_channels[channel].isOperator(clientSocket)) {
        sendMessageToClient(clientSocket, 482, channel, "You're not channel operator");
        return;
    }

    if (flag == 1) {
        _channels[channel].setInviteOnly(true);
        sendMessageToClient(clientSocket, 324, channel, "+i");
    } else if (flag == -1) {
        _channels[channel].setInviteOnly(false);
         sendMessageToClient(clientSocket, 324, channel, "-i");
    } else {
        sendMessageToClient(clientSocket, 472, channel, "+i or -i expected for Invite-only mode");
    }
}

void Server::handleTopicRestrictionMode(int clientSocket, const std::string &channel, int flag) {
    if (!_channels[channel].isOperator(clientSocket)) {
        sendMessageToClient(clientSocket, 482, channel, "You're not channel operator");
        return;
    }

    if (flag == 1) {
        _channels[channel].setTopicRestricted(true);
        sendMessageToClient(clientSocket, 324, channel, "+t");
    } else if (flag == -1) {
        _channels[channel].setTopicRestricted(false);
        sendMessageToClient(clientSocket, 324, channel, "-t");
    } else {
        sendMessageToClient(clientSocket, 472, channel, "+t or -t expected for Topic restriction mode");
    }
}

void Server::handleChannelKeyMode(int clientSocket, const std::string &channel, std::istringstream &iss, int flag) {
    if (!_channels[channel].isOperator(clientSocket)) {
        sendMessageToClient(clientSocket, 482, channel, "You must be an operator to set or remove the channel password.");
        return;
    }

    std::string modeArgs;
    if (iss >> modeArgs) {
        if (modeArgs.empty() && flag == -1) {
            _channels[channel].setPassword("");
            sendMessageToClient(clientSocket, 324, channel, "-k :Channel password removed");
        } else if (!modeArgs.empty() && flag == 1) {
            _channels[channel].setPassword(modeArgs);
            sendMessageToClient(clientSocket, 324, channel, "+k :Channel password set");
        } else if (flag == 0) {
            sendMessageToClient(clientSocket, 472, channel, "+k or -k expected to set or remove channel password");
        }
    } else {
        sendMessageToClient(clientSocket, 461, channel, "MODE :Provide a password to set or leave empty to remove");
    }
}

void Server::handleUserLimitMode(int clientSocket, const std::string &channel, std::istringstream &iss, int flag) {
    if (!_channels[channel].isOperator(clientSocket)) {
        sendMessageToClient(clientSocket, 482, channel, "You must be an operator to set or remove the user limit.");
        return;
    }

    int limit;
    if (iss >> limit && flag == 1) {
        _channels[channel].setLimit(limit);
        sendMessageToClient(clientSocket, 324, channel, "+l " + to_string(limit) + " :User limit set"); 
    } else if (flag == -1) {
        _channels[channel].setLimit(1000);
        sendMessageToClient(clientSocket, 324, channel, "-l :User limit removed"); 
    } else if (flag == 0) {
        sendMessageToClient(clientSocket, 472, channel, "+l or -l expected to set or remove user limit");
    } else {
        sendMessageToClient(clientSocket, 461, channel, "MODE :Provide a valid user limit.");
    }
}

void Server::handleOperatorPrivilegeMode(int clientSocket, const std::string &channel, std::istringstream &iss, int flag) {
    if (!_channels[channel].isOperator(clientSocket) || !_clients[clientSocket].isOper()) {
        sendMessageToClient(clientSocket, 482, channel, "You must be an operator to add or remove another operator.");
        return;
    }

    std::string targetNick;
    if (iss >> targetNick) {
        int targetSocket = findClientSocketByNick(targetNick);
        if (targetSocket != -1) {
            if (_channels[channel].isOperator(targetSocket) && flag == -1) {
                _channels[channel].removeOperator(targetSocket);
                sendMessageToClient(clientSocket, 328, channel, targetNick + " :Operator privileges removed"); 
            } else if (flag == 1) {
                _channels[channel].addOperator(targetSocket);
                sendMessageToClient(clientSocket, 328, channel, targetNick + " :Granted operator privileges");
            } else if (flag == 0) {
                sendMessageToClient(clientSocket, 472, channel, "+o or -o expected for adding/removing operator");
            }
        } else {
            sendMessageToClient(clientSocket, 401, channel, targetNick + " :No such nick/channel"); 
        }
    } else {
        sendMessageToClient(clientSocket, 461, channel, "MODE :Provide a nickname to grant/revoke operator privileges");
    }
}

void Server::handleModeCommand(int clientSocket, const std::string &arguments) {
    std::istringstream iss(arguments);
    std::string channel, mode, modeArgs;
    iss >> channel >> mode;

    if (_channels.find(channel) == _channels.end()) {
       sendMessageToClient(clientSocket, 403, channel, "No such channel");
        return;
    }

    std::string targetNick;
        int flag = 0;
    for (std::string::iterator it = mode.begin(); it != mode.end(); ++it) {
        char c = *it;
        if (c == '+') {
            flag = 1;
            continue;
        } else if (c == '-') {
            flag = -1;
            continue;
        }

 switch (c) {
            case 'i':
                handleInviteOnlyMode(clientSocket, channel, flag);
                break;
            case 't':
                handleTopicRestrictionMode(clientSocket, channel, flag);
                break;
            case 'k':
                handleChannelKeyMode(clientSocket, channel, iss, flag);
                break;
            case 'l':
                handleUserLimitMode(clientSocket, channel, iss, flag);
                break;
            case 'o':
                handleOperatorPrivilegeMode(clientSocket, channel, iss, flag);
                break;
            default:
                sendMessageToClient(clientSocket, 472, channel, std::string(1, c));
                break;
        }
        flag = 0;
    }
}