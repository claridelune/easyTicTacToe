#ifndef UDP_H
#define UDP_H

#include <functional>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <iostream>

#include "../shared/logger.hpp"

class UDPListener
{
    Logger logger;

    int socketFD;
    bool keep;
    std::function<void(const int, const std::string&, sockaddr_storage, socklen_t)> func;

    void loop();

public:
    UDPListener(const std::function<void(const int sock, const std::string& datum, sockaddr_storage addr, socklen_t addr_size)>& _func, const char* _port);

    void run();
    void stop();
    bool isRunning();
    int getSocketFD() const { return socketFD; }
};

class UDPTalker
{
    Logger logger;

    int socketFD;
    bool keep;
    int thId;
    int time;
    struct addrinfo *p;
    struct sockaddr_in hostAddr;

    std::string lastData;
    std::function<bool(const std::string& datum)> func;

    void request(const std::string& message);
    void sendUDP(const std::string& message);

public:
    UDPTalker(int timeout, const std::string& hostIP, const uint16_t hostPort, std::function<bool(const std::string& datum)> eval);

    std::string sendNReceive(const std::string& message);
};

void sendString(const int socket, const std::string& message, sockaddr_storage addr, socklen_t addr_size);

char getSum(const std::string& message);

bool checkSum(const std::string& message);

#endif
