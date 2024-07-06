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

enum Role {
    ADMIN = 0x1,
    PLAYER = 0x2,
    TRAINER = 0x3
};

class ServerManager {
    private:
        Logger* _logger;

        Socket* _socketServer;

        AdminServer* _adminServer;
        PlayerServer* _playerServer;
        TrainerServer* _trainerServer;

        std::unordered_map<Role, Server*> _servers;

        void loop();
        void flush();

    public:
        ServerManager();
        ~ServerManager();
        
        void run(size_t port, std::function<void()> handler);

        void stop();
};

#endif