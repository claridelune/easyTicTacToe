#ifndef SERVER_MAIN_PLAYER_SERVER_H
#define SERVER_MAIN_PLAYER_SERVER_H

#include "server.hpp"
#include "player.hpp"

class PlayerServer : public Server { 
    public:
        std::string emit() override;
        void subscribe(std::string data) override;

        void broadcast(std::string payload) override;
};

#endif