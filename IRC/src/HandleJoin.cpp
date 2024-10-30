/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleJoin.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 05:07:35 by root              #+#    #+#             */
/*   Updated: 2024/10/21 05:08:15 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleJoinCommand(int clientSocket, const std::string &arguments) {
    std::string channel, password;
    std::istringstream iss(arguments);
    std::vector<std::string> userDetails;
    std::string detail;
    std::string message;

    while (iss >> detail) {
        userDetails.push_back(detail);
    }
    std::string nickname = _clients[clientSocket].getNickName();
    std::string serverName = getServerName();
    if (userDetails.empty()) {
        message = ":" + serverName + " 461 " + nickname + " JOIN :Not enough parameters\r\n";
        sendMessageToClient(clientSocket, message);
        return;
    }

    channel = userDetails[0];

    if (userDetails.size() > 1) {
        password = userDetails[1];
    }

    if (_channels.find(channel) == _channels.end()) {
        _channels[channel] = Channel(channel);
        if (!password.empty()) {
            _channels[channel].setPassword(password);
        } 
    } else if (_channels[channel].isInviteOnly()) {
        message = ":" + serverName + " 473 " + nickname + " " + channel + " :Cannot join channel (+i)\r\n";
        sendMessageToClient(clientSocket, message);
        return ;
    }else {
        if (!_channels[channel].getPassword().empty() && _channels[channel].getPassword() != password) {
            message =  ":" + serverName + " 475 " + nickname + " " + channel + " :Cannot join channel (+k)\r\n";
            sendMessageToClient(clientSocket, message);
            return;
        }
    }

    _channels[channel].addClient(clientSocket);

    if (_channels[channel].clientCount() == 1) {
        _channels[channel].addOperator(clientSocket);
    }
    _clients[clientSocket].setChannel(channel);
    message = ":" + nickname + " JOIN :" + channel + "\r\n";
    sendMessageToClient(clientSocket, message);
    message = ":" + nickname + " JOIN :" + channel + "\r\n";
    sendMessageToChannel(channel, message, clientSocket);
    std::cout << "Client " << clientSocket << " joined channel: " << channel << std::endl;
}
