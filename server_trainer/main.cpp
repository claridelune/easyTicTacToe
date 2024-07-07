#include "manager.hpp"
#include "processor.hpp"
#include "client.hpp"
#include "server.hpp"

#include "../udp/udp.hpp"
#include "../neural_network/dataHandler.hpp"
#include "../neural_network/neuralNetwork.hpp"

#include "../shared/logger.hpp"
#include "../shared/socket.hpp"
#include "../shared/utils.hpp"

#define IP "127.0.0.1"
#define UDP_PORT 3490

#include <iostream>

int currSeq = 0;

std::vector<std::string> data;

ProcessorOpts clientOpts {
    uuid(),
    "127.0.0.1",
    4490
};

bool evaluateDatum(const std::string& datum)
{
    if (!checkSum(datum) || (datum[0] != 'd' && datum[0] != 'f'))
        return false;

    if (datum[0] == 'f')
        return true;

    int seq = binaryToInt(datum.substr(1, 4));
    return currSeq == seq;
}

int main() {
    // Receive Data
    UDPTalker talker(2, IP, UDP_PORT, &evaluateDatum);

    for (; true; currSeq++)
    {
        std::string message;
        message.push_back('r');
        message += intToBinary(currSeq);

        int cs = getSum(message);
        message.push_back(cs);

        auto resp = talker.sendNReceive(message);

        std::cout << "Message: " << resp << std::endl;

        if (resp[0] == 'f')
            break;
        
        data.push_back(resp.substr(5, 10));
    }

    /* Logger* logger = new Logger("ServerTrainer");
    Socket* socket = new Socket();
    TrainerClient* client = new TrainerClient(socket, clientOpts);
    TrainerServer* server = new TrainerServer(socket);

    ServerManager* manager = new ServerManager(client, server);

    manager->run([&]() {
        logger->info("Trainer Server is running...");
    });

    delete manager;
    delete server;
    delete client;
    delete socket;
    delete logger; */
};
