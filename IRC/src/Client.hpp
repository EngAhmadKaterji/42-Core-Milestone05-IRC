#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include "Channel.hpp"

enum State
{
    HANDSHAKE,
    LOGIN,
};
class Client {
private:
    int _socket;
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _hostname;
    std::string _invitedChannel;
    std::string _message;
    int _invitedBy;
    State _state;
    int _noOfTries;
    bool _isoper;
    bool _isAdmin;
    bool _acceptInvite;
    bool _isInviteState;
    std::vector<Channel> _channels;
public:
    Client();
    ~Client();
    Client(int socket, const std::string& name);
    Client(const Client &other);
    Client& operator=(const Client &other);

    int getSocket() const;

    int getNoOfTries() const;
    void setNoOfTries();
    void setNoOfTries(int no);
    void setChannel(const std::string &channel);
    std::string getChannel() const;

    bool acceptInvite() const;
    void setAcceptInvite(bool acceptInvite);

    std::string getMessage() const ;
    void setMessage(const std::string &message);
    void eraseMessage();
    
    bool isAdmin() const;
    void setAdmin(bool isAdmin);

    std::string getNickName() const;
    void setNickName(const std::string &nickName);

    std::string getUsername() const;
    void setUsername(const std::string &username);

    std::string getRealName() const;
    void setRealName(const std::string &realname);

    std::string getHostname() const;
    void setHostname(const std::string &hostname);

    std::string getInvitedChannel() const;
    void setInvitedChannel(const std::string &invitedChannel);

    int getInvitedBy() const;
    void setInvitedBy(int invitedBy);

    State getState() const;
    void setState(const State &state);

    bool isOper() const;
    void setOper(bool isOper);

    bool isInviteState() const;
    void setInviteState(bool inviteState);

    const std::vector<Channel>& getChannels() const;
    void addChannel(const Channel &channel);
    void removeChannel(const std::string &channelName);

};

#endif