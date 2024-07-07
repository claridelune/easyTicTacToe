#include "client.hpp"

void TrainerClient::initialize() { 
    configure();
    auto& ctx = getOptions();
    _socket->initialize(ctx.port, ctx.ipAddress);
    _socket->configureClient();
    _socket->connectToServer();
}

void TrainerClient::configure() { }
