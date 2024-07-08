#ifndef SERVER_TRAINER_CLIENT_H
#define SERVER_TRAINER_CLIENT_H

#include <stdexcept>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>
#include <unordered_map>

#include "processor.hpp"

#include "../shared/socket.hpp"
#include "../neural_network/neuralNetwork.hpp"

enum class Key {
    SEND,
    RECEIVE
};

struct KeyHash {
    std::size_t operator()(Key k) const {
        return static_cast<std::size_t>(k);
    }
};

class Observer {
    public:
        virtual void onActionRequestAdded(Key key, const Request& req) = 0;
        virtual void onActionResponseAdded(Key key, const Response& res) = 0;
};

class ObserverMap {
    private:
        std::unordered_map<Key, Request, KeyHash> _requests;
        std::unordered_map<Key, Response, KeyHash> _responses;
        std::vector<Observer*> _observers;

        void notify(Key key, const Request& req) {
            for (Observer* observer : _observers) {
                observer->onActionRequestAdded(key, req);
            }
        }

        void notify(Key key, const Response& res) {
            for (Observer* observer : _observers) {
                observer->onActionResponseAdded(key, res);
            }
        }   

    public:
        void addObserver(Observer* observer) {
            _observers.push_back(observer);
        }

        void addAction(Key key, const Request& req) {
            _requests[key] = req;
            notify(key, req);
        }

        void addAction(Key key, const Response& res) {
            _responses[key] = res;
            notify(key, res);
        }
};

class TrainerClient : public TrainerProcessor, public Observer {
    private:
        ProcessorOpts _extraServerOptions;
        ProcessorOpts _extraClientOptions;

        Socket* _extraClient;
        std::atomic<bool> _stopExtraClient;
        std::unique_ptr<std::thread> _runningExtraClient;

        ObserverMap _actionsMap;

        bool _requiredServerInstance;
        bool _requiredServerInstanceFirstTime;
        bool _requiredServerDisposed;

        bool _receiveData = false;

    public:
        TrainerClient(Socket* socket, NeuralNetwork *nn, ProcessorOpts& opts): TrainerProcessor(socket, nn), _extraClient(nullptr) {
            setOptions(opts);
        }            

        ProcessorOpts& getExtraServerOptions() { return _extraServerOptions; }
        void setExtraServerOptions(ProcessorOpts opts) { _extraServerOptions = opts; }
        ProcessorOpts& getExtraClientOptions() { return _extraClientOptions; }
        void setExtraClientOptions(ProcessorOpts opts) { _extraClientOptions = opts; }   

        void startExtraClient();
        void stopExtraClient();
        void loopExtraClient();

        bool requiredServerInstance() { return _requiredServerInstance;}
        bool requiredServerDisposed() { return _requiredServerDisposed; }
        bool requiredServerInstanceFirstTime() { return _requiredServerInstanceFirstTime; }
        bool receiveData() { return _receiveData; }

        void setRequiredServerInstance(bool v) { _requiredServerInstance = v;}
        void setRequiredServerDisposed(bool v) { _requiredServerDisposed = v; }
        void setRequiredServerInstanceFirstTime(bool v) { _requiredServerInstanceFirstTime = v; }
        void setReceiveData(bool v) { _receiveData = v; }

        void initialize() override;
        void configure() override;

        void send(Response res, Socket* socket = nullptr) override {
            Socket* currentSocket = (socket == nullptr) ?  _socket : socket;

            int sockId = currentSocket->getIdentity();
            currentSocket->sender(sockId, [&]() -> std::string {
                json jsonMeta = {
                    {"action", res.action},
                    {"credential", {
                        {"role", TRAINER_ROLE},
                        {"name", _options.uid}
                    }},
                    {"data", res.data}
                };
                
                std::string resStr = jsonMeta.dump();
                return resStr;
            });
        }

        Request receive(Socket* socket = nullptr) override {
            Socket* currentSocket = (socket == nullptr) ?  _socket : socket;

            int sockId = currentSocket->getIdentity();
            Request req;
            currentSocket->consumer(sockId, [&](std::string buffer) {
                json payload = json::parse(buffer);
                req.action = payload["action"];
                req.data = payload.value("data", json::object());
            });

            return req;
        }

        void onActionRequestAdded(Key key, const Request& req) override {
            if (_extraClient != nullptr) {
                std::cout << "[EXTRA CLIENT] receiving data..." << std::endl;
                receive(_extraClient);
            }
        }

        void onActionResponseAdded(Key key, const Response& res) override {
            if (_extraClient != nullptr) {
                std::cout << "[EXTRA CLIENT] sending data..." << std::endl;

                // FIX: ESTA COSA ENVIA PERO NO SE PORQUE NO LLEGA AL SERVER

                int sockId = _extraClient->getIdentity();
                _extraClient->sender(sockId, []() {
                    return R"(
                        {
                            "action": "join",
                            "credential": {
                                "name": "asdasdasdsadsa"
                            }
                        }
                    )";
                });
                // send(res, _extraClient);

                // END
            }
        }

        void config(Request req);
        void join(Request req);
        void data(Request req);
        void train(Request req);
        Response predict(Request req);
        Response keepAlive(Request req);

        void initializeTrainingData(std::vector<std::string>& data);
};

#endif
