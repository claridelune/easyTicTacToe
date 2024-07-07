#include "manager.hpp"
#include <iostream>

void ServerManager::loop() {
    _client->initialize();

    json jsonMeta = {
        {"action", "join"},
        {"credential", {
            {"role", 3},
            {"name", "aasdadasdas"}
        }}
    };

    std::string req = jsonMeta.dump();

    _client->send(req);
    std::string res = _client->receive();

    std::cout << res << std::endl;

    while(true) {
        std::string res = _client->receive();
        std::cout << "ITERATOR: " << res << std::endl;
    }
}

void ServerManager::run(std::function<void()> handler) {
    handler();
    loop();
}