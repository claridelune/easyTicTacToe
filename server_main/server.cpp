#include "server.hpp"

Server::Server() {}

Server::~Server() {}

void Server::loop() {
    while(true) {

    }
}

void Server::listen(size_t port, std::function<void()> handler) {
    handler();
    loop();
}

void Server::start() {}

void Server::next() {}

void Server::close() {}