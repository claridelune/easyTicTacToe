#include <iostream>
#include "manager.hpp"

ServerManager::ServerManager() {
    _adminServer = new AdminServer();
    _playerServer = new PlayerServer();
    _trainerServer = new TrainerServer();    

    _roleListeners[Role::ADMIN] = {
        _adminServer,
        std::bind(&AdminServer::emit,_adminServer),
        std::bind(&AdminServer::subscribe, _adminServer, std::placeholders::_1)
    };

    _roleListeners[Role::PLAYER] = {
        _playerServer,
        std::bind(&PlayerServer::emit,_playerServer),
        std::bind(&PlayerServer::subscribe, _playerServer, std::placeholders::_1)
    };
    _roleListeners[Role::TRAINER] = {
        _trainerServer,
        std::bind(&TrainerServer::emit,_trainerServer),
        std::bind(&TrainerServer::subscribe, _trainerServer, std::placeholders::_1)
    };
}

ServerManager::~ServerManager() {}

void ServerManager::loop() {
    while(true) {
        int socketId = _socketServer->accept();
        // struct sockaddr_in  socketAddr;
        // char socketIp[INET_ADDRSTRLEN];
        // inet_ntop(AF_INET, &socketAddr.sin_addr, socketIp, INET_ADDRSTRLEN);

        assign(socketId);    
    }
}

void ServerManager::assign(int socketId) {
    auto listener = [this, socketId](int rol) {
        auto fn = _roleListeners.find(static_cast<Role>(rol));
        if (fn != _roleListeners.end()) {
            auto& meta = fn->second;
            Server* instance = std::get<0>(meta);
            auto emit = std::get<1>(meta);
            auto subscribe = std::get<2>(meta);

            _socketServer->consumer(socketId, subscribe);
            _socketServer->sender(socketId, emit);
        }
    };

    _socketServer->auth(socketId, listener);
}

void ServerManager::flush() {}

void ServerManager::run(size_t port, std::function<void()> handler) {
    _socketServer = new Socket(port);
    _socketServer->configure();

    handler();
    loop();
}

void ServerManager::start() {
    json jsonMeta = {
        {"identity", "testIdentity"},
        {"ipAddress", "testIpAdress"},
        {"isLeader", true}
    };
    
    std::string payload = jsonMeta.dump();
    _trainerServer->broadcast(payload);
}

void ServerManager::next() {
    // new generation
}

void ServerManager::stop() {
    int socketIdentity = _socketServer->getIdentity();
    _socketServer->close(socketIdentity);
}
