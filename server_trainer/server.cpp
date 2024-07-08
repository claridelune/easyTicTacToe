#include "server.hpp"

void TrainerServer::initialize() {
    configure();
    auto& ctx = getOptions();
    _socket->initialize(ctx.port, ctx.ipAddress);
    _socket->configureServer();
 }

void TrainerServer::configure() { 
    registerVoidEndpoint("join", std::bind(&TrainerServer::join, this, std::placeholders::_1));
}

void TrainerServer::join(Request req) {
    // Response res;
    // res.action = req.action;

    // return res;
}
