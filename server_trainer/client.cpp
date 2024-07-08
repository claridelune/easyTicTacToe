#include "client.hpp"
#include <algorithm>

void TrainerClient::initialize() { 
    configure();
    auto& ctx = getOptions();
    _socket->initializeExt(ctx.port, ctx.ipAddress);
    _socket->configureClient();

    _actionsMap.addObserver(this);

    do {
        bool isConnected = _socket->connectToServer();
        if (isConnected) break;
    
        std::cout << "[CLIENT] >> trying to connect to the server..." << std::endl;
        usleep(5000 * 1000);
    } while(true);

    std::cout << "[CLIENT] >> connected to the server correctly." << std::endl;
}

void TrainerClient::startExtraClient() {
    if (_runningExtraClient == nullptr || !_runningExtraClient->joinable()) {
        std::cout << "[EXTRA CLIENT]  Starting client..." << std::endl;
        _stopExtraClient = false;
        _runningExtraClient = std::make_unique<std::thread>(&TrainerClient::loopExtraClient, this);
    }

            // ProcessorOpts extraOpts {
        //     opts.uid,
        //     opts.ipAddress,
        //     extraPort
        // };

        //  if (_extraClientSocket == nullptr) {
        //     _extraClientSocket = new Socket();
        //     _extraClientSocket->initialize(extraOpts.port, extraOpts.ipAddress);
        //     _extraClientSocket->configureClient();
        //  }

        // _isConnectedToServer = false;

        // do {
        //     std::cout << "INTENTANDO CONEXION AL SERVER" << std::endl;
        //     usleep(5000 * 1000);
        //     _isConnectedToServer = _extraClientSocket->connectToServer();            
        // }while(!_isConnectedToServer);
            
        // std::cout << "CONEXION AL SERVER CORRECTO" << std::endl;

        // Response res = { "join" };
        // send(res, _extraClientSocket);
}

void TrainerClient::stopExtraClient() {
    if (_runningExtraClient != nullptr && _runningExtraClient->joinable()) {
        std::cout << "[EXTRA CLIENT]  Shutting down client...." << std::endl;
        _stopExtraClient = true;
        _runningExtraClient->join();
        _runningExtraClient.reset();

        if (_extraClient == nullptr)
            return;

        int clientSockId = _extraClient->getIdentity();
        if (clientSockId != -1) {
            _extraClient->close(clientSockId);
            _extraClient->setIdentity(-1);
        }
        
        delete _extraClient;
    }
}

void TrainerClient::loopExtraClient() {
    auto& opts = getExtraClientOptions();
    _extraClient = new Socket();
    _extraClient->initializeExt(opts.port, opts.ipAddress);
    _extraClient->configureClient();

    do {
        bool isConnected = _extraClient->connectToServer();
        if (isConnected) break;
    
        std::cout << "[EXTRA CLIENT] >> trying to connect to the server..." << std::endl;
        usleep(5000 * 1000);
    } while(true);

    std::cout << "[EXTRA CLIENT] connected to the server correctly." << std::endl;

    // while(!_stopExtraClient) {
    //     Request req = _server->receive();
    //     if (req.action.empty()) continue;

    //     Response res = _server->subscribe(req);
        
    //     if (res.action == RESPONSE_VOID)
    //         continue;

    //     _server->send(res);
    // }
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

    if (isLeader) {
        _requiredServerInstance = true;
        _requiredServerInstanceFirstTime = true;
        _requiredServerDisposed = false;

        ProcessorOpts extraOpts {
            opts.uid,
            opts.ipAddress,
            extraPort
        };

        setExtraServerOptions(extraOpts);
        stopExtraClient();
    } else {
        _requiredServerInstance = false;        
        _requiredServerInstanceFirstTime = false;
        _requiredServerDisposed = true;

        ProcessorOpts extraOpts {
            opts.uid,
            opts.ipAddress,
            extraPort
        };
        
        setExtraClientOptions(extraOpts);
        startExtraClient();

        // FIX ME:  remove me
        usleep(5000 * 1000);

        std::cout << "[EXTRA CLIENT] >> Waiting while the server connects..." << std::endl;

        // end

        Response res = { "join" };
        _actionsMap.addAction(Key::SEND, res);
    }
}

void TrainerClient::join(Request req) {
    // Response res;
    // res.action = req.action;

    // return res;
}

Response TrainerClient::predict(Request req) {
    Response res;
    std::string board = req.data["board"];
    auto moves = _nn->predict(dataHandler.getBoard(board));

    std::vector<int> ind(moves.size());
    for (int i = 0; i < ind.size(); i++)
    {
        ind[i] = i;
    }
    sort(ind.begin(), ind.end(), [&moves](int x, int y) {
        return moves[x] > moves[y];
    });

    int move = -1;
    for (auto i : ind)
    {
        if (board[i] == '0')
        {
            move = i;
            break;
        }
    }

    res.action = req.action;
    res.data = {{"move", move}};
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
    // if (_isConnectedToServer)  {
    //     _nn->run(train_data, train_labels);
        
    //     // { action, datta }
    //     Response res = { "train" };
    //     send(res, _extraClientSocket);
    //     Request req = receive(_extraClientSocket);
    //     // servidor responde

    //     std::cout << req.message << std::endl;
    // }
}

void TrainerClient::initializeTrainingData(std::vector<std::string>& data) {
    dataHandler.format_training_data(data, train_data, train_labels);
    dataHandler.normalize_data(train_data);
}
