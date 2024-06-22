#include "udp.h"
#include <vector>

#define PORT "3490"

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

signed main()
{
    initializeData();

    listening = true;

    UDPListener listener(&UDPHandler, PORT);

    listener.run();

    while (listening) {}

    listener.stop();
    
    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}