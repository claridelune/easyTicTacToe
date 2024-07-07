#include "manager.hpp"
#include "processor.hpp"
#include "client.hpp"
#include "server.hpp"

#include "../shared/logger.hpp"
#include "../shared/socket.hpp"

ProcessorOpts clientOpts {
    "127.0.0.1",
    4490
};

int main() {
    Logger* logger = new Logger("ServerTrainer");
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
    delete logger;
};
