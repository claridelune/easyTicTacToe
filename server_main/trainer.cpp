#include "trainer.hpp"

void TrainerServer::configure() {
    registerEndpoint("join", std::bind(&TrainerServer::join, this, std::placeholders::_1));
    registerEndpoint("keepAlive", std::bind(&TrainerServer::handleKeepAlive, this, std::placeholders::_1));
}

void TrainerServer::run() {
    std::thread keepAliveThread(&TrainerServer::keepAlive, this);
    keepAliveThread.detach();
}

void TrainerServer::stop() {
    _isRunning = false;
}
void TrainerServer::keepAlive() {
    while (_isRunning) {
        sleep(10);

        auto now = std::chrono::steady_clock::now();
        for (auto it = _context.connectedTrainers.begin(); it != _context.connectedTrainers.end();) {
            const std::string& trainerName = it->first;
            int sockId = it->second;

            _context.sender(sockId, [&]() -> std::string {
                    json response = {
                        {"action", "keepAlive"},
                        {"message", "Are you alive?"}
                    };
                    std::string res = response.dump();
                    return res;
            });

            auto lastResponseTime = _lastKeepAliveResponse[trainerName];
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastResponseTime).count();
            if (duration > 20) {
                std::cout << "Trainer disconnected: " << trainerName << std::endl;
                _context.connectedTrainers.erase(it++);
                reassignLeader();
            } else {
                ++it;
            }
        }
    }
}

void TrainerServer::reassignLeader() {
    if (!_context.connectedTrainers.empty()) {
        auto newLeader = _context.connectedTrainers.begin()->first;
        _config->setLeader(newLeader);
        std::cout << "New leader assigned: " << newLeader;

        sendConfiguration();
    }
}

void TrainerServer::sendConfiguration() {
    json responseJson;
    responseJson["action"] = "config";
    responseJson["message"] = "New configuration.";
    responseJson["data"] = json::array();

    for (const auto& item : _config->all()) {
        json jsonArray = {
            {"identity", item.identity},
            {"ipAddress", item.ipAddress},
            {"isLeader", item.isLeader}
        };
        responseJson["data"].push_back(jsonArray);
    }

    std::string responseStr = responseJson.dump();

    for (const auto& trainer : _context.connectedTrainers) {
        _context.sender(trainer.second, [&responseStr]() -> std::string {
            return responseStr;
        });
    }
}

Response TrainerServer::join(Request request) {
    Response response;

    _config->set({request.sockName, "0.0.0.0", false});
    _lastKeepAliveResponse[request.sockName] = std::chrono::steady_clock::now();

    sendConfiguration();

    response.action = "data";
    return response;
}

Response TrainerServer::handleKeepAlive(Request request) {
    Response response;
    _lastKeepAliveResponse[request.sockName] = std::chrono::steady_clock::now();

    response.action = "void";

    return response;
}

