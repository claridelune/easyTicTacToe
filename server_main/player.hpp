#ifndef SERVER_MAIN_PLAYER_SERVER_H
#define SERVER_MAIN_PLAYER_SERVER_H

#include "server.hpp"
#include "player.hpp"

class PlayerServer : public Server { 
    private:
        Context& _context;

        Context& context() {
            return _context;
        }

    public:
        PlayerServer(Context& context) : _context(context) { 
            initialize(); 
        }

        void configure() override;

        void addClient(const std::string name, const int sockId) override {
            Context& ctx = context();
            ctx.connectedPlayers[name] = sockId;
        }

        Response predict(Request request);
};

#endif