#ifndef SERVER_MAIN_SERVER_MANAGER_H
#define SERVER_MAIN_SERVER_MANAGER_H

#include <vector>
#include <unordered_map>
#include <functional>

#include "logger.hpp"
#include "socket.hpp"
#include "server.hpp"
#include "admin.hpp"
#include "player.hpp"
#include "trainer.hpp"

// class AdminServer;
// class PlayerServer;
// class TrainerServer;

enum Role {
    ADMIN = 0x1,
    PLAYER = 0x2,
    TRAINER = 0x3
};

struct Context {
    Socket* socket;
    AdminServer* admin;
    PlayerServer* player;
    TrainerServer* trainer;
};

class ServerManager {
    private:
        Logger* _logger;

        Context _context;

        std::unordered_map<Role, Server*> _servers;

        void loop();
        void flush();

    public:
        ServerManager();
        ~ServerManager();
        
        void run(size_t port, std::function<void()> handler);

        void start();
        void next();
        void stop();
};

#endif