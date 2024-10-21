#include "Server.hpp"

void Server::handleWhoCommand(int clientSocket, const std::string &arguments){
    std::vector<std::string> channels = split(arguments, ' ');
    std::ostringstream message;
    for (size_t i = 0 ; i < channels.size(); ++i){
        const std::string &channelName = channels[i];
        if (_channels.find(channelName) != _channels.end()){
            const Channel &channel = _channels[channelName];
            if (channel.getClients().find(clientSocket) != channel.getClients().end()){
                message << "Channel: " << channel.getName() << " \nUsers: ";
                std::set<int> clientsInSet = channel.getClients();
                std::vector<int> clientsInChannel(clientsInSet.begin(), clientsInSet.end());
                for (size_t j = 0 ; j < clientsInChannel.size(); ++j){
                    message << _clients[clientsInChannel[j]].getNickName();
                    if (j < clientsInChannel.size() - 1){
                        message << ", ";
                    }
                }
                message << "\r\n";
            }else{
                sendMessageToClient(clientSocket, 401, "No such channel or you are not in channel " + channelName + "\r\n");
            }
        }else{
            sendMessageToClient(clientSocket, 401, "No such channel: " + channelName + "\r\n");
        }
    }
    if (!message.str().empty()){
        sendMessageToClient(clientSocket, 200, message.str());
    } else{
        sendMessageToClient(clientSocket, 461, "WHO", ":Syntax error, expected: WHO <channel name>\r\n");

    }
}