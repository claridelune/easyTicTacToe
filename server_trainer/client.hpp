#ifndef SERVER_TRAINER_CLIENT_H
#define SERVER_TRAINER_CLIENT_H

#include <stdexcept>
#include <unistd.h>

#include "processor.hpp"

#include "../shared/socket.hpp"
#include "../neural_network/neuralNetwork.hpp"

class TrainerClient : public TrainerProcessor {
    private:
        Socket* _extraClientSocket;

        ProcessorOpts _extraServerOptions;

        bool _requiredServerInstance;
        bool _requiredServerInstanceFirstTime;
        bool _requiredServerDisposed;

        bool _isConnectedToServer;
        bool _receiveData = false;


    public:
        TrainerClient(Socket* socket, NeuralNetwork *nn, ProcessorOpts& opts): TrainerProcessor(socket, nn), _extraClientSocket(nullptr) {
            setOptions(opts);
        }            

        ProcessorOpts& getExtraServerOptions() { return _extraServerOptions; }
        void setExtraServerOptions(ProcessorOpts opts) { _extraServerOptions = opts; }

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

        void config(Request req);
        void join(Request req);
        void data(Request req);
        void train(Request req);
        Response predict(Request req);
        Response keepAlive(Request req);

        void initializeTrainingData(std::vector<std::string>& data);
};

#endif
