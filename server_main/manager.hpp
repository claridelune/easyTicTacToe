#ifndef SERVER_MAIN_SERVER_MANAGER_H
#define SERVER_MAIN_SERVER_MANAGER_H

#include <vector>
#include <unordered_map>
#include <functional>

#include <nlohmann/json.hpp>

#include "socket.hpp"
#include "config.hpp"
#include "server.hpp"
#include "admin.hpp"
#include "player.hpp"
#include "trainer.hpp"

using json = nlohmann::json;

enum Role {
    ADMIN = 0x1,
    PLAYER = 0x2,
    TRAINER = 0x4
};

class ServerManager {
    private:
        Socket* _socketServer;

        AdminServer* _adminServer;
        PlayerServer* _playerServer;
        TrainerServer* _trainerServer;

        using EmitFunc = std::string (Server::*)();
        using SuscribeFunc = void (Server::*)(std::string);

        std::unordered_map<Role,
            std::tuple<
                Server*,
                std::function<const std::string()>,
                std::function<void(char* data)>
            >
        > _roleListeners;

        void loop();
        void assign(int socketId);
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