#include "manager.hpp"
#include "processor.hpp"
#include "client.hpp"
#include "server.hpp"

#include "../udp/udp.hpp"

#include "../shared/logger.hpp"
#include "../shared/socket.hpp"
#include "../shared/utils.hpp"

ProcessorOpts clientOpts {
    uuid(),
    "127.0.0.1",
    4490
};

int main() {
    Logger* logger = new Logger("ServerTrainer");
    Socket* socket = new Socket();
    TrainerClient* client = new TrainerClient(socket, clientOpts);
    TrainerServer* server = new TrainerServer(socket);
    UDPTalker* talker = new UDPTalker(2, IP, UDP_PORT, &evaluateDatum);

    ServerManager* manager = new ServerManager(client, server, talker);

    manager->run([&]() {
        logger->info("Trainer Server is running...");
    });

    delete manager;
    delete server;
    delete client;
    delete talker;
    delete socket;
    delete logger;
};
