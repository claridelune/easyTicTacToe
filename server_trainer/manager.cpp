#include "manager.hpp"

#include <iostream>

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
    }
}

void ServerManager::run(std::function<void()> handler) {
    handler();
    loop();
}