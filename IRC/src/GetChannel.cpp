#include "Server.hpp"

std::string Server::getClientChannel(int clientSocket){
        std::map<std::string, Channel>::iterator channelIt = _channels.begin();
        for(; channelIt != _channels.end(); ++channelIt){
        if (channelIt->second.getClients().find(clientSocket) != channelIt->second.getClients().end())
            return channelIt->first;
        }
        return "";
}