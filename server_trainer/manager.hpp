#ifndef SERVER_TRAINER_MANAGER_H
#define SERVER_TRAINER_MANAGER_H

#include <functional>
#include <thread>
#include <atomic>
#include <chrono>

#include <nlohmann/json.hpp>

#define IP "127.0.0.1"
#define UDP_PORT 7490

#include "client.hpp"
#include "server.hpp"
#include "../udp/udp.hpp"

bool evaluateDatum(const std::string& datum);

class ServerManager {
    private:
        std::atomic<bool> _stopServer;
        std::unique_ptr<std::thread> _runningServer;

        TrainerClient* _client;
        TrainerServer* _server;
        UDPTalker * talker;

        void startServer();
        void stopServer();
        void loopServer();
        void loop();
        void receiveData();

    public:
        ServerManager(TrainerClient* client, TrainerServer* server, UDPTalker* udp) :
        _client(client), 
        _server(server), 
        talker(udp),
        _stopServer(false), 
        _runningServer(nullptr) {}

        ~ServerManager() {
            stopServer();
         }

        void run(std::function<void()> handler);
};

#endif
