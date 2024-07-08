#include "udp.hpp"
#include <cassert>

UDPListener::UDPListener(const std::function<void(const int, const std::string&, sockaddr_storage, socklen_t)> &_func, const char* _port) :
    func(_func), keep(false), logger("UDPListener")
{
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, _port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(201);
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((socketFD = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }
        if (bind(socketFD, p->ai_addr, p->ai_addrlen) == -1) {
            close(socketFD);
            perror("listener: bind");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        exit(202);
    }

    freeaddrinfo(servinfo);
}

void UDPListener::loop()
{
    while (keep)
    {
        const int maxbuff = 1000;
        char buff[maxbuff+1];
        struct sockaddr_storage addr;
        socklen_t addr_size = sizeof addr;

        int numbytes;

        addr_size = sizeof addr;
        if ((numbytes = recvfrom(socketFD, buff, maxbuff, 0, (struct sockaddr *)&addr, &addr_size)) == -1) {
            perror("recvfrom");
            exit(203);
        }
        buff[numbytes] = '\0';
        std::string datum(buff, numbytes);

        logger.info("Received: " + datum);
        
        std::thread th(func, socketFD, datum, addr, addr_size);
        th.detach();
    }
}

void UDPListener::run()
{
    if (keep == true)
        return;
    keep = true;

    std::thread th(&UDPListener::loop, this);
    th.detach();
}

void UDPListener::stop()
{
    keep = false;
}

bool UDPListener::isRunning()
{
    return keep;
}

void sendString(const int socket, const std::string& message, sockaddr_storage addr, socklen_t addr_size)
{
    sendto(socket, message.c_str(), message.size(), 0, (struct sockaddr *) &addr, addr_size);
}

char getSum(const std::string &message)
{
    assert(!message.empty());

    int x = message[0];
    for (int i = 1; i < message.size(); i++)
    {
        x ^= (int)(message[i]);
    }

    return x;
}

bool checkSum(const std::string &message)
{
    assert(message.size() >= 2);

    const int check = message[message.size()-1];
    int x = message[0];
    for (int i = 1; i < message.size()-1; i++)
    {
        x ^= (int)message[i];
    }
    return x == check;
}

void UDPTalker::request(const std::string& message)
{
    int myId = thId;
    while (keep && myId == thId)
    {
        sendUDP(message);
        std::this_thread::sleep_for(std::chrono::seconds(time));
    }
}

void UDPTalker::sendUDP(const std::string &message)
{
    socklen_t addrLen = sizeof(hostAddr);
    sendto(socketFD, message.c_str(), message.size(), 0, (struct sockaddr*)&hostAddr, addrLen);
    logger.info("Sent: " + message);
}

UDPTalker::UDPTalker(int timeout, const std::string &hostIP, const uint16_t hostPort, std::function<bool(const std::string& datum)> eval) :
    time(timeout), keep(false), func(eval), thId(0), logger("UDPTalker")
{

    // struct addrinfo hints, *servinfo;
    int rv;

    socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFD < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&hostAddr, 0, sizeof(hostAddr));
    hostAddr.sin_family = AF_INET;
    hostAddr.sin_port = htons(hostPort);
    hostAddr.sin_addr.s_addr = inet_addr(hostIP.c_str());

    /* memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM; */

    /* if ((rv = getaddrinfo(hostIP.c_str(), hostPort.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(211);
    } */

    // loop through all the results and make a socket
    /* for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((socketFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        exit(212);
    } */
}

std::string UDPTalker::sendNReceive(const std::string &message)
{
    keep = true;
    std::thread th(&UDPTalker::request, this, message);
    th.detach();
    while (keep)
    {
        const int maxbuff = 1000;
        char buff[maxbuff+1];

        int numbytes;

        if ((numbytes = recvfrom(socketFD, buff, maxbuff, 0, nullptr, nullptr)) == -1) {
            perror("recvfrom");
            exit(213);
        }
        buff[numbytes] = '\0';
        std::string datum(buff, numbytes);

        logger.info("Received: " + datum);
        
        if (func(datum))
        {
            keep = false;
            lastData = datum;
        }
    }
    thId++;
    return lastData;
}
