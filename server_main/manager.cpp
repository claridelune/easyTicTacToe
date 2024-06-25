#include <iostream>
#include "manager.hpp"

ServerManager::ServerManager() {
    _database = new Database();

    _adminServer = new AdminServer();
    _playerServer = new PlayerServer();
    _trainerServer = new TrainerServer();    

    _roleListeners.insert({Role::ADMIN, std::bind(&AdminServer::subscribe, _adminServer, std::placeholders::_1)});
    _roleListeners.insert({Role::PLAYER, std::bind(&PlayerServer::subscribe, _playerServer, std::placeholders::_1)});
    _roleListeners.insert({Role::TRAINER, std::bind(&TrainerServer::subscribe, _trainerServer, std::placeholders::_1)});
}

ServerManager::~ServerManager() {
    delete _database;
    delete _adminServer;
    delete _playerServer;
    delete _trainerServer;
}

void ServerManager::loop() {
    while(true) {
        int socketId = _socketServer->accept();
        // struct sockaddr_in  socketAddr;
        // char socketIp[INET_ADDRSTRLEN];
        // inet_ntop(AF_INET, &socketAddr.sin_addr, socketIp, INET_ADDRSTRLEN);

        Request request;
        Response response;

        _socketServer->consumer(socketId, [&](char* buffer) {
            json payload = json::parse(buffer);
            request.action = payload["action"];
            request.credential = payload["credential"];
            request.data = payload.value("data", json::object());
        });

        auto userRef = _database->find(request.credential);

        if (userRef != _database->all().end()) {
            const UserCredential& user = *userRef;
             auto fn = _roleListeners.find(static_cast<Role>(user.role));
             if (fn != _roleListeners.end()) {
                response = fn->second(request);
                _socketServer->sender(socketId, [&]() -> std::string {
                    json jsonMeta = {
                        {"action", response.action},
                        {"message", response.message},
                        {"data", response.data}
                    };
    
                    return jsonMeta.dump();
                });
             }
        }  

        _socketServer->close(socketId);
    }
}

void ServerManager::flush() {}

void ServerManager::run(size_t port, std::function<void()> handler) {
    _socketServer = new Socket(port);
    _socketServer->configure();

    handler();
    loop();
}

void ServerManager::start() {

}

void ServerManager::next() {
    // new generation
}

void ServerManager::stop() {
    int socketIdentity = _socketServer->getIdentity();
    _socketServer->close(socketIdentity);
}
