#include "Server.hpp"

void Server::handlePassCommand(int clientSocket, const std::string &password) {
    std::string message;
    std::cout << "Password prompt sent to client." << std::endl;
    std::cout << "Attempt " << (_clients[clientSocket].getNoOfTries() + 1) 
              << ": Received password: '" << password << "'" << std::endl;

    if (password == _password) {
        message = "Password accepted. You are now logged in.\n";
        sendMessageToClient(clientSocket, message);
        _clients[clientSocket].setState(LOGIN);
        _clients[clientSocket].setNoOfTries(0);
        std::cout << "Password accepted for client " << clientSocket << "." << std::endl;
        if (_clients.size() == 1){
            _clients[clientSocket].setOper(true);
        }
    } else {
        _clients[clientSocket].setNoOfTries();
        message = "Error 464: Password not correct! Please enter the password again.\r\n";
        sendMessageToClient(clientSocket, message);
        std::cout << "Incorrect password for client " << clientSocket << "." << std::endl;

        if (_clients[clientSocket].getNoOfTries() == 3) {
            message = "Too many failed attempts. Please try again later.\n";
            sendMessageToClient(clientSocket, message);
            _clients[clientSocket].setNoOfTries(0);
            removeClient(clientSocket);
        }
    }

}

void Server::handleNewConnection() {
    std::string message;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if (clientSocket < 0) {
        std::cerr << "Error: accept() failed: " << strerror(errno) << std::endl;
        return;
    }

    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0) {
        std::cerr << "Error: fcntl() failed: " << strerror(errno) << std::endl;
        removeClient(clientSocket);
        return;
    }

    _clients[clientSocket] = Client(clientSocket, "");
    _clients[clientSocket].setNoOfTries(0);

    struct pollfd clientPollFd = {clientSocket, POLLIN, 0}; 
    _pollFds.push_back(clientPollFd);

    message = "Welcome! Please enter the password to log in.\n";
    sendMessageToClient(clientSocket, message);

    std::cout << "New client connected from " << inet_ntoa(clientAddr.sin_addr) << " on socket " << clientSocket << "." << std::endl;
}
