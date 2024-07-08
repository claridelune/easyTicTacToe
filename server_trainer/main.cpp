#include <unordered_map>

#include "manager.hpp"
#include "processor.hpp"
#include "client.hpp"
#include "server.hpp"

#include "../udp/udp.hpp"

#include "../shared/logger.hpp"
#include "../shared/socket.hpp"
#include "../shared/utils.hpp"
#include "../neural_network/neuralNetwork.hpp"

#define CLIENT_MAIN_PORT 4490
#define CLIENT_TRAINER_PORT 4491

std::unordered_map<std::string, std::string> parseArgs(int argc, char* argv[]) {
    std::unordered_map<std::string, std::string> args;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        size_t pos = arg.find('=');
        if (pos != std::string::npos) {
            std::string key = arg.substr(0, pos);
            std::string value = arg.substr(pos + 1);
            args[key] = value;
        }
    }

    return args;
}

int main(int argc, char* argv[]) {
    auto args = parseArgs(argc, argv);

    std::string uid = uuid();
    std::string clientMainIp = "127.0.0.1";
    std::string clientTrainerIp = "127.0.0.1";

    if (args.find("--main") != args.end())
        clientMainIp = args["--main"];

    if (args.find("--trainer") != args.end())
        clientTrainerIp = args["--trainer"];

    ProcessorOpts clientMainOpts {
        uid,
        clientMainIp,
        CLIENT_MAIN_PORT
    };

    ProcessorOpts clientTrainerOpts {
        uid,
        clientTrainerIp,
        CLIENT_TRAINER_PORT
    };

    Logger* logger = new Logger("ServerTrainer");
    Socket* socketClient = new Socket();
    Socket* socketServer = new Socket();
    NeuralNetwork* nn = new NeuralNetwork(9, 3, 9, 30, 0.01); 
   
    TrainerClient* client = new TrainerClient(socketClient, nn, clientMainOpts);
    TrainerServer* server = new TrainerServer(socketServer, nn);
    UDPTalker* talker = new UDPTalker(2, IP, UDP_PORT, &evaluateDatum);

    ServerManager* manager = new ServerManager(client, server, talker);

    manager->run([&]() {
        logger->info("Trainer Server is running...");
    });

    delete manager;
    delete server;
    delete client;
    delete talker;
    delete socketServer;
    delete socketClient;
    delete logger;
};
