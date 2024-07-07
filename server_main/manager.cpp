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
    int cliSock, maxSd, activity, 
    srvSock = _socketServer->getIdentity();
    sockaddr_in address = _socketServer->getAddress();
    fd_set readfds;
    std::set<int> acceptedClients;

    while(true) {
        FD_ZERO(&readfds);
        FD_SET(srvSock, &readfds);
        maxSd = srvSock;

        _logger->info("Ingresando nueva conexion");
        for (auto it = acceptedClients.begin(); it != acceptedClients.end(); ++it) {
            int sd = *it;
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > maxSd) maxSd = sd;
        }

        _logger->info("Iniciando seleccion");
        activity = select(maxSd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            _logger->error("Error al seleccionar");
        }

        if (FD_ISSET(srvSock, &readfds)) {
            int newSock = _socketServer->accept();
            // ntohs(address.sin_port)
            _logger->info("New Connection, socket fs is " + std::to_string(newSock) + ", IP is: " + std::string(inet_ntoa(address.sin_addr)));
            acceptedClients.insert(newSock);
        }

        _logger->info("Buscando conexion para request and response");
        for (auto it = acceptedClients.begin(); it != acceptedClients.end();) {
            int sd = *it;
            
            _logger->info("Estableciendo conexion con: " + std::to_string(sd));

            if (FD_ISSET(sd, &readfds)) {  
                Request request;
                Response response;

                _logger->info("Iniciando consumer and sender con: " + std::to_string(sd));

                int valread = _socketServer->consumer(sd, [&](std::string buffer) {
                    _logger->info("REQUEST: " + buffer);
                    json payload = json::parse(buffer);
                    request.sockId = sd;
                    request.sockRole = payload["credential"]["role"];
                    request.sockName = payload["credential"]["name"];
                    
                    request.action = payload["action"];
                    request.data = payload.value("data", json::object());
                });

                if (valread == 0) {
                    getpeername(sd, (struct sockaddr *)&address, (socklen_t *)sizeof(address));
                    std::cout << "Host desconectado, IP: " << inet_ntoa(address.sin_addr) << ", puerto: " << ntohs(address.sin_port) << std::endl;
                    _socketServer->close(sd);
                    it = acceptedClients.erase(it); 
                } else {
                    auto srv = _servers.find(static_cast<Role>(request.sockRole));
                    if (srv != _servers.end()) {
                        srv->second->addClient(request.sockName, request.sockId);
                        response = srv->second->subscribe(request);

                        if (response.action != "void") {
                            _socketServer->sender(sd, [&]() -> std::string {
                                json jsonMeta = {{"action", response.action},
                                                {"message", response.message},
                                                {"data", response.data}};

                                std::string res = jsonMeta.dump();
                                _logger->info("RESPONSE: " + res);

                                return res;
                          });
                        }
                    }

                    ++it;
                }
            } else {
                ++it;
            }
        }
    }

    _socketServer->close(srvSock);
}

void ServerManager::flush() {}

void ServerManager::run(size_t port, std::function<void()> handler) {
    _socketServer = new Socket(port);
    _socketServer->configureServer();

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

    _trainerServer->run();

    handler();
    loop();
}

void ServerManager::stop() {
    int socketIdentity = _socketServer->getIdentity();
    _socketServer->close(socketIdentity);
    _trainerServer->stop();
}
