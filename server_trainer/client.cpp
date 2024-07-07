#include "client.hpp"

void TrainerClient::initialize() { 
    configure();
    auto& ctx = getOptions();
    _socket->initialize(ctx.port, ctx.ipAddress);
    _socket->configureClient();
    _socket->connectToServer();
}

void TrainerClient::configure() { 
    registerVoidEndpoint("config", std::bind(&TrainerClient::config, this, std::placeholders::_1));
    registerEndpoint("predict", std::bind(&TrainerClient::predict, this, std::placeholders::_1));
    registerEndpoint("keepAlive", std::bind(&TrainerClient::keepAlive, this, std::placeholders::_1));
}

void TrainerClient::config(Request req) {
    
}

Response TrainerClient::predict(Request req) {
    Response res;
    res.action = req.action;

    return res;
}

Response TrainerClient::keepAlive(Request req) {
    Response res;
    res.action = req.action;
    return res;
}
