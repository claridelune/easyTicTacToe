#ifndef SERVER_TRAINER_CLIENT_H
#define SERVER_TRAINER_CLIENT_H

#include "processor.hpp"

#include "../shared/socket.hpp"

class TrainerClient : public TrainerProcessor {
    public:
        TrainerClient(Socket* socket, ProcessorOpts& opts): TrainerProcessor(socket) {
            setOptions(opts);
        }  

        void initialize() override;
        void configure() override;

        void config(Request req);
        Response predict(Request req);
        Response keepAlive(Request req);
};

#endif