#ifndef SERVER_TRAINER_MANAGER_H
#define SERVER_TRAINER_MANAGER_H

#include <functional>
#include <thread>
#include <atomic>
#include <chrono>

#include <nlohmann/json.hpp>

#include "client.hpp"
#include "server.hpp"

class ServerManager {
    private:
        std::atomic<bool> _stopServer;
        std::unique_ptr<std::thread> _runningServer;

        TrainerClient* _client;
        TrainerServer* _server;

        void startServer();
        void stopServer();
        void loopServer();
        void loop();

    public:
        ServerManager(TrainerClient* client, TrainerServer* server) :
        _client(client), 
        _server(server), 
        _stopServer(false) , 
        _runningServer(nullptr) { }

        ~ServerManager() {
            stopServer();
         }

        void run(std::function<void()> handler);
};

#endif