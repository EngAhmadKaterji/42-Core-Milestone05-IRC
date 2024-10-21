#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <set>
#include <string>

class Channel {
private:
    std::set<int> _clients;        
    std::string _name;         
    std::string _password;         
    std::set<int> _operators;     
    std::string _topic;            
    bool _inviteOnly;              
    bool _topicRestricted;       
    int _limit;

public:
    Channel();
    Channel(const std::string &name);
    ~Channel();
    Channel(const Channel &other);
    Channel& operator=(const Channel &other);

    void setName(const std::string &name);
    const std::string& getName() const;

    bool isOperator(int clientSocket) const;
    void addOperator(int clientSocket);
    void removeOperator(int clientSocket);
    bool isClient(int clientSocket) const;

    void addClient(int clientSocket);
    void removeClient(int clientSocket);
    bool isFull() const;
    const std::set<int>& getClients() const;
    int clientCount() const;

    void setTopic(int clientSocket, const std::string &topic);
    const std::string& getTopic() const;

    void setInviteOnly(bool inviteOnly);
    bool isInviteOnly() const;
    void setTopicRestricted(bool restricted);
    bool isTopicRestricted() const;

    void setPassword(const std::string &password);
    const std::string& getPassword() const;

    bool isEmpty() const;
    void setLimit(int limit);
    int getLimit() const;
};

#endif
