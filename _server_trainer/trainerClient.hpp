#ifndef TRAINER_CLIENT_H
#define TRAINER_CLIENT_H

#include "../shared/client.hpp"
#include "../shared/utils.hpp"
// #include "state.hpp"
#include "trainerServer.hpp"

#include <memory>
#include <string>

#define ROL 3

class TrainerClient : public Client {
public:
    TrainerClient(const std::string& ip, const size_t port, Logger* logger);
    void configure() override;
    
    Request updateConfiguration(Response response);
    Request startTraining(Response response);

    void loop();

    // void setState(State* state);

    std::string _uuid;
    std::string currentLeaderIP;

    // State* currentState;
    std::unique_ptr<Socket> tcpHandler;
    std::unique_ptr<TrainerServer> server;
    size_t _port;
};

#endif // TRAINER_CLIENT_H
