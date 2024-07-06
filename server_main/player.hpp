#ifndef SERVER_MAIN_PLAYER_SERVER_H
#define SERVER_MAIN_PLAYER_SERVER_H

#include "server.hpp"
#include "player.hpp"

class PlayerServer : public Server { 
    private:
        void addClient(const std::string name, const int sockId) override {
            auto records = context().connectedPlayers;
            auto player = records.find(name);
            if (player == records.end())
                records.insert({ name, sockId });
        }

    public:
        PlayerServer(Context& context): Server(context) { 
            initialize(); 
        }

        void configure() override;

        Response predict(Request request);
};

#endif