#ifndef TRAINER_CLIENT_H
#define TRAINER_CLIENT_H

#include "../shared/client.hpp"

class PlayerClient : public Client {
    int lastMove = -1;
    public:
        PlayerClient(const std::string& ip, const size_t port, Logger* logger) : Client(ip, port, logger) {
            initialize();
        }

        void configure() override;

        void onJoinResponse(Response response);

        void onPredictResponse(Response Response);

        int getLastMove();
};

#endif
