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
#include <cassert>
#include <iostream>

class UDPListener
{
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
    int socketFD;
    bool keep;
    int thId;
    int time;
    struct addrinfo *p;

    std::string lastData;
    std::function<bool(const std::string& datum)> func;

    void request(const std::string& message);
    void sendUDP(const std::string& message);

public:
    UDPTalker(int timeout, const std::string& hostIP, const std::string& hostPort, std::function<bool(const std::string& datum)> eval);

    std::string sendNReceive(const std::string& message);
};

void sendString(const int socket, const std::string& message, sockaddr_storage addr, socklen_t addr_size);

char getSum(const std::string& message);

bool checkSum(const std::string& message);

int binaryToInt(const std::string& binaryNumber);

std::string intToBinary(int num);