#include <csignal>
#include <iostream>
#include <cstdlib>
#include "Server.hpp"

bool g_server = false;

void signalHandler(int signum) {
    (void) signum;
    g_server = true;
    std::cout << "\nInterrupt signal (" << signum << ") received. Stopping server..." << std::endl;
}
int ft_getport(char *argc)
{
    int port;
    char *buffer;

    if (*argc == '\0')
        return (1);
    port = strtol(argc, &buffer, 10);
    if (*buffer !='\0')
        {
            std::cout << "Error input: port must contain only digits" << std::endl;
            return(1);
        }
    if (port < 0 || port > 65535)
    {
        std::cout << "Error input: port is out of range [0; 65535]" << std::endl;
        return (1);
    }
    return (0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return (1);
    }
    signal(SIGINT, signalHandler);
    if (ft_getport(argv[1]))
        return (1);
    try {
        Server server(argv[1], argv[2]);
        server.start();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
