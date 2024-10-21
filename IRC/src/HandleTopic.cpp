#include "Server.hpp"

void Server::handleTopicCommand(int clientSocket, const std::string &message) { 
    std::string channel;
    std::string topic;

    size_t colonPos = message.find(":");
    if (colonPos + 1 >= message.length()) {
        sendMessageToClient(clientSocket, 432, "Usage: TOPIC <channel> :<new topic>\n");
        return;
    }

    if (colonPos == 0 && message.length() > 0){
       channel = message;
       trim(channel);
        std::map<std::string, Channel>::iterator channelIt = _channels.find(channel);
            if (channelIt == _channels.end() || 
        channelIt->second.getClients().find(clientSocket) == channelIt->second.getClients().end()) {
        sendMessageToClient(clientSocket, 442, "You're not in this channel.\n");
    } else {
        sendMessageToClient(clientSocket, 332, channel, getChannelTopic(channel));
    }
    return;
    }
    channel = message.substr(0, colonPos);
    topic = message.substr(colonPos + 1);

    trim(channel);
    trim(topic);

    std::map<std::string, Channel>::iterator channelIt = _channels.find(channel);

    if (channelIt == _channels.end() || 
        channelIt->second.getClients().find(clientSocket) == channelIt->second.getClients().end()) {
        sendMessageToClient(clientSocket, 442, "You're not in this channel.\n");
        return;
    }

    if (channelIt->second.isTopicRestricted()) {
        if(channelIt->second.isOperator(clientSocket)) {
            channelIt->second.setTopic(clientSocket, topic);
            sendMessageToChannel(channel, "The topic in channel " + channel + " has been changed to: " + topic, clientSocket);
            sendMessageToClient(clientSocket, "Topic in channel " + channel + " changed to: " + topic + "\r\n");
        } else {
            sendMessageToClient(clientSocket, 482, "Only Operators can set the topic.\n"); 
        }
    } else {
        channelIt->second.setTopic(clientSocket, topic);
        sendMessageToChannel(channel, "The topic in channel " + channel + " has been changed to: " + topic, clientSocket);
        sendMessageToClient(clientSocket, "Topic in channel " + channel + " changed to: " + topic + "\r\n");
    }
}

std::string Server::getChannelTopic(const std::string &channel) const {
    std::map<std::string, Channel>::const_iterator it = _channels.find(channel);
    if (it != _channels.end() && !it->second.getTopic().empty()) {
        return it->second.getTopic();
    }
    return "No topic set for this channel.\n";
}
