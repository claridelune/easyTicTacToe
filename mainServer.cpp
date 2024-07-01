#include "udp.h"
#include <vector>

#include "server_main/manager.hpp"
#include "server_main/logger.hpp"

#define UDP_PORT "3490"
#define TCP_PORT 4490

Logger logger("MainServer");

bool listening;

std::vector<std::string> data; // each string is 9 chars (board) (???)

void initializeData()
{
    data.push_back("100120020");
}

void UDPHandler(const int socket, const std::string& datum, sockaddr_storage addr, socklen_t addr_size)
{
    if (!checkSum(datum))
        return;
    
    if (datum[0] == 'r')
    {
        int seq = binaryToInt(datum.substr(1, 4));

        std::string message;
        message.push_back('d');
        message += datum.substr(1, 4);
        message += data[seq];

        auto cs = getSum(message);
        message.push_back(cs);

        sendString(socket, message, addr, addr_size);
    }
}

void runUDPServer() {
    initializeData();

    listening = true;

    UDPListener listener(&UDPHandler, UDP_PORT);

    logger.info("UDP Server is running...");

    listener.run();

    while (listening) {}

    listener.stop();
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void runTCPServer() {
    ServerManager* manager = new ServerManager();
    manager->run(TCP_PORT, []() {
        logger.info("TCP Server is running...");
    });

    delete manager;
}

signed main()
{
    std::thread udpThread(runUDPServer);
    std::thread tcpThread(runTCPServer);

    udpThread.join();
    tcpThread.join();

    return 0;
}