#include "Server.hpp"

void Server::handleTopicCommand(int clientSocket, const std::string &message) {
    std::string channel;
    std::string topic;
    std::string nickname = _clients[clientSocket].getNickName();
    std::string serverName = getServerName();  // This should be the server's name or ID

    size_t colonPos = message.find(":");

    // If no colon is found, the message is just the channel name, so we send the current topic.
    if (colonPos == std::string::npos) {
        channel = message;
        trim(channel);

        std::map<std::string, Channel>::iterator channelIt = _channels.find(channel);
        if (channelIt == _channels.end() || 
            channelIt->second.getClients().find(clientSocket) == channelIt->second.getClients().end()) {
            sendMessageToClient(clientSocket, 442, "You're not in this channel.\n");
        } else {
            std::string currentTopic = getChannelTopic(channel);
            // Send the current topic to the client (332 message)
            sendMessageToClient(clientSocket, 332, serverName + " " + channel + " :" + currentTopic + "\r\n");
        }
        return;
    }

    // Extract channel and topic from the message
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

    // Check if the channel has restricted topic setting
    if (channelIt->second.isTopicRestricted()) {
        if (channelIt->second.isOperator(clientSocket)) {
            // Set the new topic in the channel
            channelIt->second.setTopic(clientSocket, topic);
            // Send the correct TOPIC command message to all clients in the channel
            std::string topicMessage = ":" + nickname + "!" + _clients[clientSocket].getUserName() + "@"
                                    + _clients[clientSocket].getHostName() + " TOPIC " + channel + " :" + topic + "\r\n";
            sendMessageToChannel(channel, topicMessage, clientSocket, 1);
            // Inform the client about the topic change
            sendMessageToClient(clientSocket, "Topic in channel " + channel + " changed to: " + topic + "\r\n");
        } else {
            sendMessageToClient(clientSocket, 482, "Only Operators can set the topic.\n"); 
        }
    } else {
        // If no restrictions, change the topic
        channelIt->second.setTopic(clientSocket, topic);
        // Send the correct TOPIC command message to all clients in the channel
        std::string topicMessage = ":" + nickname + "!" + _clients[clientSocket].getUserName() + "@"
                                    + _clients[clientSocket].getHostName() + " TOPIC " + channel + " :" + topic + "\r\n";
        sendMessageToChannel(channel, topicMessage, clientSocket, 1);
        // Inform the client about the topic change
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
