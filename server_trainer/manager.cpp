#include "manager.hpp"

#include <iostream>
int currSeq = 0;
std::vector<std::string> data;

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
    auto& opts = _client->getExtraServerOptions();
    _server->setOptions(opts);
    _server->initialize();

    while(!_stopServer) {
        Request req = _server->receive();
        if (req.action.empty()) continue;

        Response res = _server->subscribe(req);
        
        if (res.action == RESPONSE_VOID)
            continue;

        _server->send(res);
    }
}

void ServerManager::loop() {
    _client->initialize();

    Response frsReq { "join" };

    _client->send(frsReq);

    while(true) {
        Request req = _client->receive();
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
            std::cout << "START SERVER" << std::endl;
            startServer();
            _client->setRequiredServerInstanceFirstTime(false);
            _client->setRequiredServerInstance(false);
        }

        if (_client->requiredServerDisposed()) {
            std::cout << "STOP SERVER" << std::endl;
            stopServer();
            _client->setRequiredServerDisposed(false);
        }

        if (res.action == RESPONSE_VOID)
            continue;

        _client->send(res);
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

