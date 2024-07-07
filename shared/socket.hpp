#ifndef SERVER_MAIN_SOCKET_H
#define SERVER_MAIN_SOCKET_H

#define SOCKET_ERROR -1

#include <functional>
#include <string>
#include <cstring>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <unistd.h> 
#include <stdexcept>

#include "utils.hpp"
#include "logger.hpp"

inline int socketAccept(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
    return accept(sockfd, addr, addrlen);
}

inline int socketClose(int fd) {
    return close(fd);
}

class Socket {
    private:
        sockaddr_in _socketAddr;
        int _socketId;

        Logger* _logger;

    public:
        Socket(const size_t port);
        Socket(const std::string& ip, const size_t port);
        ~Socket();

        void configureServer();
        void configureClient();
        
        int getIdentity();
        sockaddr_in getAddress();

        int accept();
        void connectToServer();
        int consumer(const int socketId, const std::function<void(std::string b)> handler);
        void sender(const int socketId, const std::function<const std::string()> handler);
        int close(int socketId);
};

#endif
