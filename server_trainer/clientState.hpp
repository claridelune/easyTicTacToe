#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

#include "serverState.hpp"
#include "trainerClient.hpp"
#include "trainerServer.hpp"
#include <nlohmann/json.hpp>

class ClientState : public State {
public:
    void handleConfig(TrainerClient* client, const std::string& config) override {
        auto configData = nlohmann::json::parse(config);
        for (const auto& item : configData) {
            if (item["isLeader"] && item["identity"] == client->_uuid) {
                client->server = std::make_unique<TrainerServer>(client->_port);
                client->server->configure();
                client->setState(new ServerState());
                return;
            } else if (!item["isLeader"] && item["identity"] == client->_uuid) {
                std::string leaderIP = item["ipAddress"];
                if (leaderIP != client->currentLeaderIP) {
                    client->currentLeaderIP = leaderIP;
                    client->tcpHandler = std::make_unique<Socket>(leaderIP, client->_port);
                    client->tcpHandler->connectToServer();
                }
            }
        }
    }

    void handleAction(TrainerClient* client, const std::string& action) override {
        if (action == "start" || action == "continue") {
            client->startTraining(action);
        }
    }
};

#endif // CLIENT_STATE_H
