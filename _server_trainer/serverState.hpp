// #ifndef SERVER_STATE_H
// #define SERVER_STATE_H

// #include "state.hpp"
// #include "trainerClient.hpp"
// #include "trainerServer.hpp"
// #include "clientState.hpp"
// #include <nlohmann/json.hpp>

// class ServerState : public State {
// public:
//     void handleConfig(TrainerClient* client, const std::string& config) override {
//         auto configData = nlohmann::json::parse(config);
//         for (const auto& item : configData) {
//             if (item["isLeader"] && item["identity"] == client->_uuid) {
//                 client->server = std::make_unique<TrainerServer>(client->_port);
//                 client->server->configure();
//                 client->setState(new ServerState());
//                 return;
//             } else if (!item["isLeader"] && item["identity"] == client->_uuid) {
//                 client->setState(new ClientState());
//                 return;
//             }
//         }
//     }

//     void handleAction(TrainerClient* client, const std::string& action) override {
//         if (action == "start" || action == "continue") {
//             client->server->startTraining(action);
//         }
//     }
// };

// #endif // SERVER_STATE_H