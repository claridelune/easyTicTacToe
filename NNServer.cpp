#include "udp.h"

#define IP "127.0.0.1"
#define PORT "3490"

int currSeq = 0;

bool evaluateDatum(const std::string& datum)
{
    if (!checkSum(datum) || datum[0] != 'd')
        return false;
    
    int seq = binaryToInt(datum.substr(1, 4));

    return seq == currSeq;
}

signed main()
{
    UDPTalker talker(2, IP, PORT, &evaluateDatum);

    std::string message;
    message.push_back('r');
    message += intToBinary(currSeq);

    int cs = getSum(message);
    message.push_back(cs);

    auto resp = talker.sendNReceive(message);

    std::cout << "SENT: " << message << "\nRECEIVED: " << resp << '\n';
}