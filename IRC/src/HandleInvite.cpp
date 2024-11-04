#include "Server.hpp"
#include <sstream>
#include <vector>

void Server::handleInviteeCommand(int inviteeSocket, const std::string &message) {
    if (message == "y" || message == "Y") {
        _clients[inviteeSocket].setAcceptInvite(true);
    } else if (message == "n" || message == "N") {
        _clients[inviteeSocket].setAcceptInvite(false);
        _channels[_clients[inviteeSocket].getInvitedChannel()].removeInvitedClient(inviteeSocket);
    } else {
        _clients[inviteeSocket].setNoOfTries();
        if (_clients[inviteeSocket].getNoOfTries() < 3) {
            sendMessageToClient(inviteeSocket, "Invalid option. Please press [y/n]: ");
        } else {
            int inviterSocket = _clients[inviteeSocket].getInvitedBy();
            sendMessageToClient(inviterSocket, _clients[inviteeSocket].getNickName() + " failed to join " + _clients[inviteeSocket].getInvitedChannel() + " due to multiple invalid responses.\n");
            _clients[inviteeSocket].setNoOfTries(0);
            _clients[inviteeSocket].setInvitedChannel("");
            _clients[inviteeSocket].setAcceptInvite(false);
            _clients[inviteeSocket].setInviteState(false);
            _channels[_clients[inviteeSocket].getInvitedChannel()].removeInvitedClient(inviteeSocket);
        }
        return;
    }

    if (_clients[inviteeSocket].acceptInvite()) {
        std::string channelName = _clients[inviteeSocket].getInvitedChannel();
        sendMessageToClient(inviteeSocket, "You can joined channel " + channelName + ".\n");
        int inviterSocket = _clients[inviteeSocket].getInvitedBy();
        std::string sendmsg = ":" + getServerName() + " 341 " + _clients[inviterSocket].getNickName() + " " + _clients[inviteeSocket].getNickName() + " " + channelName + "\n";
        sendMessageToClient(inviterSocket, sendmsg);
        
        _clients[inviteeSocket].setInvitedChannel("");
        _clients[inviteeSocket].setAcceptInvite(false);
        _clients[inviteeSocket].setInviteState(false);
    } else {
        int inviterSocket = _clients[inviteeSocket].getInvitedBy();
        sendMessageToClient(inviterSocket, _clients[inviteeSocket].getNickName() + " has declined the invitation to join " + _clients[inviteeSocket].getInvitedChannel() + ".\n");
        _clients[inviteeSocket].setInvitedChannel("");
        _clients[inviteeSocket].setInviteState(false);
         _channels[_clients[inviteeSocket].getInvitedChannel()].removeInvitedClient(inviteeSocket);
    }
}

void Server::handleInviteCommand(int clientSocket, const std::string &message) {
    std::istringstream iss(message);
    std::vector<std::string> tokens;
    std::string token;
    std::string sendmsg;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.size() < 2) {
        sendmsg = ":" + getServerName() + " 461 " + _clients[clientSocket].getNickName() + " INVITE :Not enough parameters\nUse: INVITE <nickname> <channel>\n";
        sendMessageToClient(clientSocket, sendmsg);
        return;
    }

    std::string invitee = tokens[0];
    std::string channel = tokens[1];

    std::map<std::string, Channel>::iterator channelIt = _channels.find(channel);
    if (channelIt == _channels.end() || channelIt->second.getClients().find(clientSocket) == channelIt->second.getClients().end()) {
        sendMessageToClient(clientSocket, "You must be in the channel to invite others.\n");
        return;
    }

    if (!channelIt->second.isOperator(clientSocket)) {
        sendmsg = ":" + getServerName() + " 482 " + _clients[clientSocket].getNickName() + " " + channel + " :You're not a channel operator\n";
        sendMessageToClient(clientSocket, sendmsg);
        return;
    }

    if (channelIt->second.isFull()) {
        sendmsg = ":" + getServerName() + " 471 " + _clients[clientSocket].getNickName() + " " + channel + " :Cannot join channel (+l) - channel is full\n";
        sendMessageToClient(clientSocket, sendmsg);
        return;
    }

    int inviteeSocket = -1;
    std::map<int, Client>::iterator clientIt;
    for (clientIt = _clients.begin(); clientIt != _clients.end(); ++clientIt) {
        if (clientIt->second.getNickName() == invitee) {
            inviteeSocket = clientIt->first;
            break;
        }
    }

    if (inviteeSocket == -1) {
        sendmsg = ":" + getServerName() + " 401 " + _clients[clientSocket].getNickName() + " " + invitee + " :No such nick/channel\n";
        sendMessageToClient(clientSocket, sendmsg);
        return;
    }
    _channels[channel].addInvitedClient(inviteeSocket);
    sendMessageToClient(inviteeSocket, "You have been invited to join channel " + channel + ". Please press [y/n]: ");
    _clients[inviteeSocket].setInviteState(true);
    _clients[inviteeSocket].setInvitedChannel(channel);
    _clients[inviteeSocket].setInvitedBy(clientSocket);
}
