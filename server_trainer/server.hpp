#ifndef SERVER_TRAINER_SERVER_H
#define SERVER_TRAINER_SERVER_H

#include "processor.hpp"

#include "../shared/socket.hpp"
#include "../neural_network/neuralNetwork.hpp"

class TrainerServer : public TrainerProcessor {
    public:
        TrainerServer(Socket* socket, NeuralNetwork* nn): TrainerProcessor(socket, nn) {}

        void initialize() override;
        void configure() override;

        Response join(Request req);
};

#endif
