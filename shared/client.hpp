#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <unordered_map>
#include <functional>
#include <nlohmann/json.hpp>
#include "socket.hpp"
#include "logger.hpp"

using json = nlohmann::json;

struct Request {
    std::string action;
    json credential;
    json data;
};

struct Response {
    std::string action;
    std::string message;
    json data;
};

class Client {
    protected:
        std::unordered_map<std::string, std::function<Request(Response response)>> _handlers;
        Logger* logger;

        void initialize() {
            logger->info("Initializing client");
            configure();
        }

    private:
        void handleResponse(const std::string& action, Response response) {
            auto fn = _handlers.find(action);
            if (fn != _handlers.end()) {
                fn->second(response);
            } else {
                logger->error("No handler registered for action: " + action);
            }
        }

    public:
        Socket* _socketClient;

        Client(const std::string& ip, const size_t port, Logger* logger) : logger(logger) {
            logger->info("Creating socket");
            _socketClient = new Socket(ip, port);
            _socketClient->configureClient();
            _socketClient->connectToServer();
        }

        virtual ~Client() {
            int socketIdentity = _socketClient->getIdentity();
            _socketClient->close(socketIdentity);
            delete _socketClient;
        }

        virtual void configure() = 0;

        void registerHandler(std::string action, std::function<Request(Response response)> handler) {
            _handlers.insert({action, handler});
        }

        void sendRequest(Request request) {
            _socketClient->sender(_socketClient->getIdentity(), [&]() -> std::string {
                json jsonMeta = {
                    {"action", request.action},
                    {"credential", request.credential},
                    {"data", request.data}
                };
                std::string res = jsonMeta.dump();
                logger->info("Sending request: " + res);
                return res;
            });
        }

        void listen() {
            _socketClient->consumer(_socketClient->getIdentity(), [&](std::string buffer) {
                logger->info("Response: " + buffer);
                json payload = json::parse(buffer);
                Response response;
                response.action = payload["action"];
                response.message = payload["message"];
                response.data = payload.value("data", json::object());

                handleResponse(response.action, response);
            });
        }
};

#endif
