#include <iostream>
#include "manager.hpp"

ServerManager::ServerManager() {
    _logger = new Logger("ServerManager");

    _context.admin = new AdminServer();
    _context.player = new PlayerServer();
    _context.trainer = new TrainerServer();    

    _servers.insert({Role::ADMIN, _context.admin });
    _servers.insert({Role::PLAYER, _context.player });
    _servers.insert({Role::TRAINER, _context.trainer });
}

ServerManager::~ServerManager() {
    delete _context.admin;
    delete _context.player;
    delete _context.trainer;
}

void ServerManager::loop() {
    while(true) {
        int socketId = _context.socket->accept();
        // struct sockaddr_in  socketAddr;
        // char socketIp[INET_ADDRSTRLEN];
        // inet_ntop(AF_INET, &socketAddr.sin_addr, socketIp, INET_ADDRSTRLEN);

        Request request;
        Response response;

        _context.socket->consumer(socketId, [&](char* buffer) {
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
            _context.socket->sender(socketId, [&]() -> std::string {
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
    _context.socket = new Socket(port);
    _context.socket->configure();

    handler();
    loop();
}

void ServerManager::start() {

}

void ServerManager::next() {
    // new generation
}

void ServerManager::stop() {
    int socketIdentity = _context.socket->getIdentity();
    _context.socket->close(socketIdentity);
}
