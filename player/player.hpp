#ifndef TRAINER_CLIENT_H
#define TRAINER_CLIENT_H

#include "../shared/client.hpp"

class PlayerClient : public Client {
    public:
        PlayerClient(const std::string& ip, const size_t port, Logger* logger) : Client(ip, port, logger) {
            initialize();
        }

        void configure() override;

        Request onJoinResponse(Response response);
};

#endif