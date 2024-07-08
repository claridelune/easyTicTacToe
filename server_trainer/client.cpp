#include "client.hpp"

void TrainerClient::initialize() { 
    configure();
    auto& ctx = getOptions();
    _socket->initialize(ctx.port, ctx.ipAddress);
    _socket->configureClient();
    _socket->connectToServer();
}

void TrainerClient::configure() { 
    registerVoidEndpoint("config", std::bind(&TrainerClient::config, this, std::placeholders::_1));
    registerEndpoint("predict", std::bind(&TrainerClient::predict, this, std::placeholders::_1));
    registerEndpoint("keepAlive", std::bind(&TrainerClient::keepAlive, this, std::placeholders::_1));
}

void TrainerClient::config(Request req) {
    auto& opts = getOptions();

    bool isLeader = false;
    for (const auto& item : req.data) {
        if (item.at("identity").get<std::string>() == opts.uid) {
            isLeader = item.at("isLeader").get<int>() == 1;
            break; 
        }
    }

    if (true) {
        _requiredServerInstance = true;
        _requiredServerInstanceFirstTime = true;
        _requiredDisposed = false;

        int serverPort = opts.port + 1;

        ProcessorOpts extraOpts {
            opts.uid,
            opts.ipAddress,
            serverPort
        };

        setExtraOptions(extraOpts);
    } else {
        _requiredServerInstance = false;        
        _requiredServerInstanceFirstTime = false;
        _requiredDisposed = true;
    }
}

Response TrainerClient::predict(Request req) {
    Response res;
    res.action = req.action;

    return res;
}

Response TrainerClient::keepAlive(Request req) {
    Response res;
    res.action = req.action;
    return res;
}
