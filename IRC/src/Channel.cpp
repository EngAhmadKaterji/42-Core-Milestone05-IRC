#include "Channel.hpp"

Channel::Channel() 
    : _name("default_channel"), _inviteOnly(false), _topicRestricted(false), _limit(100) {}

Channel::Channel(const std::string &name) 
    : _name(name), _inviteOnly(false), _topicRestricted(false), _limit(100) {}

Channel::~Channel() {}

Channel::Channel(const Channel &other) 
    : _clients(other._clients),    
      _name(other._name),              
      _password(other._password),      
      _operators(other._operators),    
      _topic(other._topic),           
      _inviteOnly(other._inviteOnly), 
      _topicRestricted(other._topicRestricted),
      _limit(other._limit) {}   

Channel& Channel::operator=(const Channel &other) {
    if (this != &other) {
        _name = other._name;
        _clients = other._clients;
        _operators = other._operators;
        _topic = other._topic;
        _password = other._password;
        _inviteOnly = other._inviteOnly; 
        _topicRestricted = other._topicRestricted;
        _limit = other._limit;
    }
    return *this;
}

void Channel::setName(const std::string &name) {
    _name = name;
}

const std::string& Channel::getName() const {
    return _name;
}

bool Channel::isOperator(int clientSocket) const {
    return _operators.find(clientSocket) != _operators.end();
}

void Channel::addOperator(int clientSocket) {
    _operators.insert(clientSocket);
}

void Channel::removeOperator(int clientSocket) {
    _operators.erase(clientSocket);
}

bool Channel::isFull() const {
    return _clients.size() >= static_cast<std::set<int>::size_type>(_limit);
}

bool Channel::isClient(int clientSocket) const {
    return _clients.find(clientSocket) != _clients.end();
}

int Channel::addClient(int clientSocket) {
    if (isInviteOnly()) {
        if (_invitedClients.find(clientSocket) == _invitedClients.end()) {
        return 1;
    }
    }
    if (_limit > 0 && _clients.size() >= static_cast<size_t>(_limit)) {
        return 2; 
    }
    _clients.insert(clientSocket);
    if (_invitedClients.find(clientSocket) != _invitedClients.end())
        return 3;
    return 0;
}

void Channel::removeClient(int clientSocket) {
    _clients.erase(clientSocket);
}
void Channel::addInvitedClient(int invitedSocket) {
    _invitedClients.insert(invitedSocket);
}

void Channel::removeInvitedClient(int invitedSocket) {
    _invitedClients.erase(invitedSocket);
}

const std::set<int>& Channel::getClients() const {
    return _clients;
}
const std::set<int>& Channel::getInvitedClients() const{
    return _invitedClients;
}
int Channel::clientCount() const {
    return _clients.size();
}
const std::set<int>& Channel::getOperators() const{
    return _operators;
}
int Channel::operatorCount() const{
    return _operators.size();
}

void Channel::setTopic(int clientSocket, const std::string &topic) {
    if (!isTopicRestricted() || isOperator(clientSocket)) {
        _topic = topic;
    }
}

const std::string& Channel::getTopic() const {
    return _topic;
}

void Channel::setInviteOnly(bool inviteOnly) {
    _inviteOnly = inviteOnly;
}

bool Channel::isInviteOnly() const {
    return _inviteOnly;
}

void Channel::setTopicRestricted(bool restricted) {
    _topicRestricted = restricted;
}

bool Channel::isTopicRestricted() const {
    return _topicRestricted;
}

void Channel::setPassword(const std::string &password) {
    _password = password;
}

const std::string& Channel::getPassword() const {
    return _password;
}

bool Channel::isEmpty() const {
    return _clients.empty();
}

void Channel::setLimit(int limit) {
    _limit = limit;
}

int Channel::getLimit() const {
    return _limit;
}
