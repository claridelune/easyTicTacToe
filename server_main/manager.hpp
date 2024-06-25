#ifndef SERVER_MAIN_SERVER_MANAGER_H
#define SERVER_MAIN_SERVER_MANAGER_H

#include <vector>
#include <unordered_map>
#include <functional>

#include "socket.hpp"
#include "server.hpp"
#include "admin.hpp"
#include "player.hpp"
#include "trainer.hpp"

#include "../shared/database.hpp"

enum Role {
    ADMIN = 0x1,
    PLAYER = 0x2,
    TRAINER = 0x3
};

class ServerManager {
    private:
        Socket* _socketServer;

        Database* _database;

        AdminServer* _adminServer;
        PlayerServer* _playerServer;
        TrainerServer* _trainerServer;

        std::unordered_map<Role,std::function<Response(Request request)>> _roleListeners;

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