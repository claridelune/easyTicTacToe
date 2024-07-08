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
    registerVoidEndpoint("join", std::bind(&TrainerClient::join, this, std::placeholders::_1));
    registerVoidEndpoint("data", std::bind(&TrainerClient::data, this, std::placeholders::_1));
    registerVoidEndpoint("start", std::bind(&TrainerClient::train, this, std::placeholders::_1));
    registerVoidEndpoint("next", std::bind(&TrainerClient::train, this, std::placeholders::_1));
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

    int extraPort = opts.port + 1;

    if (false) {
        _requiredServerInstance = true;
        _requiredServerInstanceFirstTime = true;
        _requiredServerDisposed = false;

        ProcessorOpts extraOpts {
            opts.uid,
            opts.ipAddress,
            extraPort
        };

        setExtraServerOptions(extraOpts);
    } else {
        _requiredServerInstance = false;        
        _requiredServerInstanceFirstTime = false;
        _requiredServerDisposed = true;

        ProcessorOpts extraOpts {
            opts.uid,
            opts.ipAddress,
            extraPort
        };

         /* if (_extraClientSocket == nullptr) {
            _extraClientSocket = new Socket();
            _extraClientSocket->initialize(extraOpts.port, extraOpts.ipAddress);
            _extraClientSocket->configureClient();
         } */

        // _isConnectedToServer = false;

        /* do {
            std::cout << "INTENTANDO CONEXION AL SERVER" << std::endl;
            usleep(5000 * 1000);
            _isConnectedToServer = _extraClientSocket->connectToServer();            
        }while(!_isConnectedToServer);
            
        std::cout << "CONEXION AL SERVER CORRECTO" << std::endl;

        Response res = { "join" };
        send(res, _extraClientSocket); */
    }
}

void TrainerClient::join(Request req) {
    // Response res;
    // res.action = req.action;

    // return res;
}

Response TrainerClient::predict(Request req) {
    Response res;
    auto board = req.data;
    // int move = _nn->predict(board);

    res.action = req.action;
    res.data = {"move", 3};
    return res;
}

Response TrainerClient::keepAlive(Request req) {
    Response res;
    res.action = req.action;
    return res;
}

void TrainerClient::data(Request req) {
    _receiveData = true;
}

void TrainerClient::train(Request req) {
    if (_isConnectedToServer)  {
        _nn->run(train_data, train_labels);
        
        // { action, datta }
        Response res = { "train" };
        send(res, _extraClientSocket);
        Request req = receive(_extraClientSocket);
        // servidor responde

        std::cout << req.message << std::endl;
    }
}

void TrainerClient::initializeTrainingData(std::vector<std::string>& data) {
    dataHandler.format_training_data(data, train_data, train_labels);
    dataHandler.normalize_data(train_data);
}
