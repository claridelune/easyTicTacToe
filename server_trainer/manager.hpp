#ifndef SERVER_TRAINER_MANAGER_H
#define SERVER_TRAINER_MANAGER_H

#include <functional>

#include <nlohmann/json.hpp>

#include "client.hpp"
#include "server.hpp"

class ServerManager {
    private:
        TrainerClient* _client;
        TrainerServer* _server;

        void loop();

    public:
        ServerManager(TrainerClient* client, TrainerServer* server) : _client(client), _server(server) { }
        ~ServerManager() { }

        // void connect() {
        //     int sockId = _socketServer->getIdentity();

        //     json jsonMeta = {
        //         {"action", "join"},
        //         {"credential": {
        //             {"role": 3},
        //             {"name": "aasdadasdas"}
        //         }},
        //     };

        //     _socket->sender(sockId, [&]() -> std::string {                
        //         std::string res = jsonMeta.dump();
        //         _logger->info("RESPONSE: " + res);

        //         return res;
        //     });

        //     _socket->consumer(sockId, [&](std::string buffer) {
        //         _logger->info("REQUEST: " + buffer);
        //     });
        // }

        void run(std::function<void()> handler);
};

#endif