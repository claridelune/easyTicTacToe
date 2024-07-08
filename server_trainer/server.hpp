#ifndef SERVER_TRAINER_SERVER_H
#define SERVER_TRAINER_SERVER_H

#include "processor.hpp"

#include "../shared/socket.hpp"
#include "../neural_network/neuralNetwork.hpp"

class TrainerServer : public TrainerProcessor {

    public:
        TrainerServer(Socket* socket, NeuralNetwork* nn): TrainerProcessor(socket, nn) {}

        void initialize() override;
        void configure() override;

        int accept() {
            return _socket->accept();
        }

        void send(Response res, int sockId, Socket* socket = nullptr) override {
            Socket* currentSocket = (socket == nullptr) ?  _socket : socket;

            currentSocket->sender(sockId, [&]() -> std::string {
                json jsonMeta = {
                    {"action", res.action},
                    {"message", res.message},
                    {"data", res.data}
                };
                
                std::string resStr = jsonMeta.dump();
                return resStr;
            });
        }

        Request receive(int sockId, Socket* socket = nullptr) override {
            Socket* currentSocket = (socket == nullptr) ?  _socket : socket;

            Request req;
            currentSocket->consumer(sockId, [&](std::string buffer) {
                std::cout << "TTTTTTTTTT" << buffer << std::endl;
                json payload = json::parse(buffer);
                req.sockId = sockId;
                req.sockName = payload["credential"]["name"];
                req.action = payload["action"];
                req.data = payload.value("data", json::object());
            });

            return req;
        }

        void join(Request req);
        Response train(Request req);
};

#endif
