#include "server.hpp"

void TrainerServer::initialize() {
    configure();
    auto& ctx = getOptions();
    _socket->initialize(ctx.port, ctx.ipAddress);
    _socket->configureServer();
 }

void TrainerServer::configure() { }
