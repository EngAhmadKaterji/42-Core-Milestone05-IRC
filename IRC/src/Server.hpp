#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include <poll.h>
#include <algorithm>
#include <cctype>
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <cerrno>
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <unistd.h>
#include <string>
#include <set>

extern bool g_server;

class Server {
public:
    Server(const std::string &port, const std::string &password);
    ~Server();
    void start();
    void stop();

private:
    std::string _serverName;
    std::string _port;
    std::string _password;
    int _serverSocket;
    std::string _adminPassword;
    std::vector<pollfd> _pollFds;
    std::map<int, Client> _clients;
    std::map<std::string, Channel> _channels;
    std::map<std::string, Channel> _channelTopics;
    std::map<std::string, Channel> _channelMode;
    std::map<std::string, Channel> _channelUserLimits;

    std::string getChannelTopic(const std::string &channel) const; 
    std::string getClientChannel(int clientSocket);

    const std::string getServerName() const;
    void setServerName(const std::string &serverName);
    void initServerSocket();
    void handleNewConnection();
    void removeClient(int clientSocket);

    void sendMessageToChannel(const std::string &channel, const std::string &message, int senderSocket, int mode);
    void sendMessageToClient(int clientSocket, const std::string &message);
    void sendMessageToClient(int clientSocket, int replyCode, const std::string &message);
    void sendMessageToClient(int clientSocket, int replyCode, const std::string &target, const std::string &message);

    void handleServerCommand(int clientSocket, const std::string &arguments);
    void handleWhoCommand(int clientSocket, const std::string &arguments);
    void handleTopicCommand(int clientSocket, const std::string &message);
    void handleModeCommand(int clientSocket, const std::string &arguments);
    void handleClientMessage(int clientSocket);
    void handlePassCommand(int clientSocket, const std::string &password);
    void handleJoinCommand(int clientSocket, const std::string &channel);
    void handleListCommand(int clientSocket, const std::string &lts);
    void handleNickCommand(int clientSocket, const std::string &argument);
    void handleKickCommand(int clientSocket, const std::string &message);
    void handleInviteCommand(int clientSocket, const std::string &invitee);
    void handleInviteeCommand(int inviteeSocket, const std::string &channel);
    void handlePrvMsg(int clientSocket, const std::string &arguments);
    int sendPrivateMessage(const std::string &sender, const std::string &receiver, const std::string &message);
    void handleAdminCommand(int clientSocket, const std::string &password);
    void handlePartCommand(int clientSocket, const std::string &part);
    void handleUserCommand(int clientSocket, const std::string &user);
    void handleKillCommand(int clientSocket, const std::string &target);
    void handlePingCommand(int clientSocket, const std::string &arguments);
    void handleQuitCommand(int clientSocket, const std::string &quitMessage);
    void handleChannelMode(int clientSocket, const std::string &arguments);
    void handleOperatorPrivilegeMode(int clientSocket, const std::string &channel, std::istringstream &iss, int flag);
    void handleUserLimitMode(int clientSocket, const std::string &channel, std::istringstream &iss, int flag);
    void handleChannelKeyMode(int clientSocket, const std::string &channel, std::istringstream &iss, int flag);
    void handleTopicRestrictionMode(int clientSocket, const std::string &channel, int flag);
    void handleInviteOnlyMode(int clientSocket, const std::string &channel, int flag);
    int findClientSocketByNick(const std::string &nickname);
    void sendUpdatedNamesList(const std::string &channel);
};

#endif
