#include "manager.hpp"

#include <iostream>

void ServerManager::startServer() {
    if (_runningServer == nullptr || !_runningServer->joinable()) { 
        _stopServer = false;
        _runningServer = std::make_unique<std::thread>(&ServerManager::loopServer, this);
    }
}

void ServerManager::stopServer() {
    if (_runningServer != nullptr && _runningServer->joinable()) {
        _stopServer = true;
        _runningServer->join();
        _runningServer.reset();
    }

    int serverSockId = _server->getSockId();
    if (serverSockId != -1) {
        _server->closeSock(serverSockId);
        _server->setSockId(-1);
    }
}

void ServerManager::loopServer() {
    auto& opts = _client->getExtraOptions();
    _server->setOptions(opts);
    _server->initialize();

    while(!_stopServer) {
        std::cout << "Soy el server y estoy vivo!" << std::endl;
        Request req = _server->receive();
        Response res = _server->subscribe(req);
    }
}

void ServerManager::loop() {
    _client->initialize();

    Response frsReq {
        "join"
    };

    _client->send(frsReq);

    while(true) {
        Request req = _client->receive();
        Response res = _client->subscribe(req);

        if (res.action == RESPONSE_VOID)
            continue;

        _client->send(res);

        if (
            _client->requiredServerInstance() && 
            _client->requiredServerInstanceFirstTime() && 
            !_client->requiredDisposed()
        ) {
            std::cout << "START SERVER" << std::endl;
            startServer();
            _client->setRequiredServerInstanceFirstTime(false);
            _client->setRequiredServerInstance(false);
        }

        if (_client->requiredDisposed()) {
            std::cout << "STOP SERVER" << std::endl;
            stopServer();
            _client->setRequiredDisposed(false);
        }
    }
}

void ServerManager::run(std::function<void()> handler) {
    handler();
    loop();
}