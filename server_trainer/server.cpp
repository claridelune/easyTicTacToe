#include "server.hpp"

void TrainerServer::initialize() {
    configure();
    auto& ctx = getOptions();
    _socket->initialize(ctx.port);
    _socket->configureServer();
 }

void TrainerServer::configure() { 
    registerVoidEndpoint("join", std::bind(&TrainerServer::join, this, std::placeholders::_1));
    registerVoidEndpoint("weight", std::bind(&TrainerServer::weight, this, std::placeholders::_1));
}

void TrainerServer::join(Request req) {
    std::cout << "::::::: START JOIN ::::::::" << req.action << std::endl;
    // Response res;
    // res.action = req.action;

    // return res;
    std::cout << "::::::: END JOIN ::::::::" << req.action << std::endl;
}

void TrainerServer::weight(Request req) {
    std::cout << "::::::: START PROCESS WEIGHT ::::::::" << std::endl;

    Response res;
    res.action = "receive-weight";
    res.message = "Se estan enviado mis pesos!...";

    for(int clientSockId : _clientConnecteds) {
        send(res, clientSockId);
    }

    std::cout << "::::::: END PROCESS WEIGHT ::::::::" << std::endl;
}