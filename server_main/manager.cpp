#include <iostream>
#include "manager.hpp"

ServerManager::ServerManager() {
    _logger = new Logger("ServerManager");
}

ServerManager::~ServerManager() {
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
            _logger->info("REQUEST: " + std::string(buffer));
            json payload = json::parse(buffer);
            request.sockId = socketId;
            request.sockRole = payload["credential"]["role"];
            request.sockName = payload["credential"]["name"];
            
            request.action = payload["action"];
            request.data = payload.value("data", json::object());
        });
            
        auto srv = _servers.find(static_cast<Role>(request.sockRole));
        if (srv != _servers.end()) {
            response = srv->second->subscribe(request);
            _socketServer->sender(socketId, [&]() -> std::string {
                json jsonMeta = {
                    {"action", response.action},
                    {"message", response.message},
                    {"data", response.data}
                };
    
                std::string res = jsonMeta.dump();
                _logger->info("RESPONSE: " + res);

                return res;
            });
        }
        

        

        // _socketServer->close(socketId);
    }
}

void ServerManager::flush() {}

void ServerManager::run(size_t port, std::function<void()> handler) {
    _socketServer = new Socket(port);
    _socketServer->configure();

    auto consumerFn = std::bind(&Socket::consumer, _socketServer, std::placeholders::_1, std::placeholders::_2);
    auto senderFn = std::bind(&Socket::sender, _socketServer, std::placeholders::_1, std::placeholders::_2);

    Context context {
        consumerFn,
        senderFn
    };

    _adminServer = new AdminServer(context);
    _playerServer = new PlayerServer(context);
    _trainerServer = new TrainerServer(context);    

    _servers.insert({Role::ADMIN, _adminServer });
    _servers.insert({Role::PLAYER, _playerServer });
    _servers.insert({Role::TRAINER, _trainerServer });

    handler();
    loop();
}

void ServerManager::stop() {
    int socketIdentity = _socketServer->getIdentity();
    _socketServer->close(socketIdentity);
}
