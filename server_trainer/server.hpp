#ifndef SERVER_TRAINER_SERVER_H
#define SERVER_TRAINER_SERVER_H

#include "processor.hpp"

#include "../shared/socket.hpp"

class TrainerServer : public TrainerProcessor {
    public:
        TrainerServer(Socket* socket): TrainerProcessor(socket) {}

        void initialize() override;
        void configure() override;
};

#endif