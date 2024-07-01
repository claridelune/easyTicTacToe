#ifndef SERVER_MAIN_SERVER_H
#define SERVER_MAIN_SERVER_H

#include <string>
#include <unordered_map>
#include <functional>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Request {
    std::string action;
    std::string credential;
    json data;
};

struct Response {
    std::string action;
    std::string message;
    json data;
};

struct Client {
    int sockId;
    std::string user;
};

class Server {
    protected:
        std::vector<Client> _connectedClients;
        std::unordered_map<std::string, std::function<Response(Request request)>> _endpoints;

        void initialize() { configure(); }

    private:
        Response executeEndpoint(std::string action, Request request) {
            auto fn = _endpoints.find(action);
            if (fn != _endpoints.end()) {
                return fn->second(request);
            }

            return {
                request.action,
                "Action not found.",
                json()
            };
        }

    public:
        Server() { }

        virtual void configure() = 0;

        void registerEndpoint(std::string action, std::function<Response(Request request)> handler) {
            _endpoints.insert({action, handler});
        }

        Response subscribe(Request request) {
            return executeEndpoint(request.action, request);
        }

        void addClient(const Client& client) {
            _connectedClients.push_back(client);
        }

        const std::vector<Client>& getConnected() const {
            return _connectedClients;
        }
};

#endif