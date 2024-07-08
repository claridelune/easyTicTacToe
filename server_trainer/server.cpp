#include "server.hpp"

void TrainerServer::initialize() {
    configure();
    auto& ctx = getOptions();
    _socket->initialize(ctx.port);
    _socket->configureServer();
 }

void TrainerServer::configure() { 
    registerVoidEndpoint("join", std::bind(&TrainerServer::join, this, std::placeholders::_1));
    registerEndpoint("train", std::bind(&TrainerServer::train, this, std::placeholders::_1));
}

void TrainerServer::join(Request req) {
    std::cout << "[[[[ ESTO RECIVIENDOO ]]]]" << req.action << std::endl;
    // Response res;
    // res.action = req.action;

    // return res;
}

Response TrainerServer::train(Request req) {
    Response res;
    res.action = req.action;
    res.message = "trainn success";

    return res;
}