#ifndef TRAINER_SERVER_H
#define TRAINER_SERVER_H

#include <vector>
#include <memory>
#include "../shared/socket.hpp"

class TrainerServer {
public:
    TrainerServer(size_t port);
    void configure();
    void handleWeights(const std::vector<float>& weights);
    void startTraining();

private:
    size_t _port;
    std::unique_ptr<Socket> serverSocket;
    std::vector<int> clientSockets;
};

#endif // TRAINER_SERVER_H
