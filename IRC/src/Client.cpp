#include "Client.hpp"

Client::Client() 
    : _socket(-1), _nickname(""), _username(""), _realname(""),
      _hostname(""), _invitedChannel(""), _message(""), 
      _invitedBy(-1), _state(HANDSHAKE), _noOfTries(0), 
      _isoper(false), _isAdmin(false), 
      _acceptInvite(false), _isInviteState(false) {}

Client::~Client() {}

Client::Client(int socket, const std::string& name)
    : _socket(socket), _nickname(name), _username(""), _realname(""),
      _hostname(""), _invitedChannel(""), _message(""), 
      _invitedBy(-1), _state(HANDSHAKE), _noOfTries(0), 
      _isoper(false), _isAdmin(false), 
      _acceptInvite(false), _isInviteState(false) {}

Client::Client(const Client &other) {
    *this = other; 
}

Client& Client::operator=(const Client &other) {
    if (this != &other) {
        _socket = other._socket;
        _nickname = other._nickname;
        _username = other._username;
        _realname = other._realname;
        _hostname = other._hostname;
        _invitedChannel = other._invitedChannel;
        _message = other._message;
        _invitedBy = other._invitedBy;
        _state = other._state;
        _noOfTries = other._noOfTries;
        _isoper = other._isoper;
        _isAdmin = other._isAdmin;
        _acceptInvite = other._acceptInvite;
        _isInviteState = other._isInviteState;
        _channels = other._channels;
    }
    return *this;
}

int Client::getSocket() const { return _socket; }

void Client::setChannel(const std::string &channel) {
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->getName() == channel) {
            std::cout << "Already in channel " << channel << "." << std::endl;
            return;
        }
    }

    Channel newChannel(channel);
    _channels.push_back(newChannel);
    std::cout << "Added to channel " << channel << "." << std::endl;
}

std::string Client::getChannel() const {
    return !_channels.empty() ? _channels.front().getTopic() : "";
}

bool Client::isAdmin() const { return _isAdmin; }
void Client::setAdmin(bool isAdmin) { _isAdmin = isAdmin; }

std::string Client::getNickName() const { return _nickname; }
void Client::setNickName(const std::string &nickName) { _nickname = nickName; }

std::string Client::getUsername() const { return _username; }
void Client::setUsername(const std::string &username) { _username = username; }

std::string Client::getRealName() const { return _realname; }
void Client::setRealName(const std::string &realname) { _realname = realname; }

std::string Client::getHostname() const { return _hostname; }
void Client::setHostname(const std::string &hostname) { _hostname = hostname; }

State Client::getState() const { return _state; }
void Client::setState(const State &state) { _state = state; }

bool Client::isOper() const { return _isoper; }
void Client::setOper(bool isOper) { _isoper = isOper; }

const std::vector<Channel>& Client::getChannels() const { return _channels; }

void Client::addChannel(const Channel &channel) { _channels.push_back(channel); }
bool Client::acceptInvite() const{ return _acceptInvite;}
void Client::setAcceptInvite(bool acceptInvite){ _acceptInvite = acceptInvite;}

bool Client::isInviteState() const { return _isInviteState;}
void Client::setInviteState(bool inviteState){ _isInviteState = inviteState;}

std::string Client::getInvitedChannel() const{ return _invitedChannel;}
void Client::setInvitedChannel(const std::string &invitedChannel){ _invitedChannel = invitedChannel;}

int Client::getInvitedBy() const { return _invitedBy;}
void Client::setInvitedBy(int invitedBy){ _invitedBy = invitedBy;}

std::string Client::getMessage() const { return _message;}
void Client::setMessage(const std::string &message){ _message += message;}
void Client::eraseMessage(){ _message.clear();}
    
int Client::getNoOfTries() const { return _noOfTries;}
void Client::setNoOfTries(){ 
    if (_noOfTries <= 3)
        _noOfTries++;
        }
void Client::setNoOfTries(int n){ _noOfTries = n;}
void Client::removeChannel(const std::string &channelName) {
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->getTopic() == channelName) {
            _channels.erase(it);
            break;
        }
    }
}