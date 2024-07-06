#ifndef SERVER_MAIN_PLAYER_SERVER_H
#define SERVER_MAIN_PLAYER_SERVER_H

#include "server.hpp"
#include "player.hpp"

class PlayerServer : public Server { 
    public:
        PlayerServer() { initialize(); }

        void configure() override;

        Response predict(Request request);
};

#endif