#include "trainerClient.hpp"
#include "clientState.hpp"
#include "serverState.hpp"

TrainerClient::TrainerClient(const std::string &ip, const size_t port, Logger *logger)
    : Client(ip, port, logger), _port(port) {
    initialize();
    _uuid = uuid();
}

void TrainerClient::configure() {
    registerHandler("config", std::bind(&TrainerClient::updateConfiguration, this, std::placeholders::_1));
    registerHandler("start", std::bind(&TrainerClient::startTraining, this, std::placeholders::_1));
    registerHandler("continue", std::bind(&TrainerClient::startTraining, this, std::placeholders::_1));
    registerHandler("predict", std::bind(&TrainerClient::updateConfiguration, this, std::placeholders::_1));
}

void TrainerClient::loop() {
    Request requestJoin;
    requestJoin.action = "join";
    requestJoin.credential = {{"role", ROL}, {"name", _uuid}};
    requestJoin.data = {};

    sendRequest(requestJoin);
    listen();

    while (true) {
        listen();
    }
}

Request TrainerClient::updateConfiguration(Response response) {
    Request request;
    
    request.action = response.action;

    logger->info("updateConfiguration Response: " + response.message);

    return request;
    // currentState->handleConfig(this, response.data.dump());
}

Request TrainerClient::startTraining(Response response) {
    Request request;
    
    request.action = response.action;

    logger->info("startTraining Response: " + response.message);

    return request;
    // currentState->handleAction(this, response.action);
}

// void TrainerClient::setState(State* state) {
//     delete currentState;
//     currentState = state;
// }
