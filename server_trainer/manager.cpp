#include "manager.hpp"

#include <iostream>
int currSeq = 0;
std::vector<std::string> data;

void ServerManager::startServer() {
    if (_runningServer == nullptr || !_runningServer->joinable()) { 
        std::cout << "[SERVER]  Starting server..." << std::endl;
        _stopServer = false;
        _runningServer = std::make_unique<std::thread>(&ServerManager::loopServer, this);
    }
}

void ServerManager::stopServer() {
    if (_runningServer != nullptr && _runningServer->joinable()) {
        std::cout << "[SERVER]  Shutting down server...." << std::endl;
        _stopServer = true;
        _runningServer->join();
        _runningServer.reset();

        int serverSockId = _server->getSockId();
        if (serverSockId != -1) {
            _server->closeSock(serverSockId);
            _server->setSockId(-1);
        }
    }
}

void ServerManager::loopServer() {
    auto& opts = _client->getExtraServerOptions();
    _server->setOptions(opts);
    _server->initialize();

    while(!_stopServer) {
        int sockId = _server->accept();
        _server->addClient(sockId);
        Request req = _server->receive(sockId);
        if (req.action.empty()) continue;
        std::cout << "[SERVER] receiving data... -> action: " << req.action << std::endl; 

        Response res = _server->subscribe(req);
        
        if (res.action == RESPONSE_VOID)
            continue;

        std::cout << "[SERVER] sending data... -> action: " << req.action << std::endl; 
        _server->send(res, sockId);
    }
}

void ServerManager::loop() {
    _client->initialize();

    Response frsReq { "join" };

    _client->send(frsReq, 0);

    while(true) {
        Request req = _client->receive(0);
        if (req.action.empty()) continue;

        Response res = _client->subscribe(req);

        if (_client->receiveData()) {
            std::thread dataThread([&]() {
                receiveData();
            });
            dataThread.detach();
        }

        if (
            _client->requiredServerInstance() && 
            _client->requiredServerInstanceFirstTime() && 
            !_client->requiredServerDisposed()
        ) {
            startServer();
            _client->setRequiredServerInstanceFirstTime(false);
            _client->setRequiredServerInstance(false);
        }

        if (_client->requiredServerDisposed()) {
            stopServer();
            _client->setRequiredServerDisposed(false);
        }

        if (res.action == RESPONSE_VOID)
            continue;

        _client->send(res, 0);
    }
}

void ServerManager::receiveData()
{
    for (; true; currSeq++)
    {
        std::string message;
        message.push_back('r');
        message += intToBinary(currSeq);

        int cs = getSum(message);
        message.push_back(cs);

        auto resp = talker->sendNReceive(message);

        if (resp[0] == 'f')
            break;
        
        data.push_back(resp.substr(5, 10));
    }
    _client->initializeTrainingData(data);
    _client->setReceiveData(false);
}

void ServerManager::run(std::function<void()> handler) {
    handler();
    loop();
}

bool evaluateDatum(const std::string& datum)
{
    if (!checkSum(datum) || (datum[0] != 'd' && datum[0] != 'f'))
        return false;

    if (datum[0] == 'f')
        return true;

    int seq = binaryToInt(datum.substr(1, 4));
    return currSeq == seq;
}

