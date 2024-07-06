#ifndef TRAINER_CLIENT_H
#define TRAINER_CLIENT_H

#include "../shared/client.hpp"

class TrainerClient : public Client {
    public:
        TrainerClient(const std::string& ip, const size_t port, Logger* logger) : Client(ip, port, logger) {
            initialize();
        }

        void configure() override;

        void onJoinResponse(Response response);
};

#endif
