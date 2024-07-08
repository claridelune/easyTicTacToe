#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <unordered_map>
#include <functional>
#include <nlohmann/json.hpp>
#include <variant>
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
        using Handler = std::variant<std::function<void(Response response)>, std::function<Request(Response response)>>;
        std::unordered_map<std::string, Handler> _handlers;
        Logger* logger;

        void initialize() {
            logger->info("Initializing client");
            configure();
        }

    private:
        void handleResponse(const std::string& action, Response response) {
            auto fn = _handlers.find(action);
            if (fn != _handlers.end()) {
                std::visit([&](auto&& handler) { handler(response); }, fn->second);
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

            do {
                bool isConnected = _socketClient->connectToServer();
                if (isConnected) break;
                logger->info("trying to connect to the server...");
                usleep(5000 * 1000);
            } while(true);
            logger->info("connected to the server correctly.");
        }

        virtual ~Client() {
            int socketIdentity = _socketClient->getIdentity();
            _socketClient->close(socketIdentity);
            delete _socketClient;
        }

        virtual void configure() = 0;

        void registerVoidHandler(std::string action, std::function<void(Response response)> handler) {
            _handlers.insert({action, handler});
        }

        void registerRequestHandler(std::string action, std::function<Request(Response response)> handler) {
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
