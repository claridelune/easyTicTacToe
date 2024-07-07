#ifndef SERVER_MAIN_SERVER_H
#define SERVER_MAIN_SERVER_H

#include <string>
#include <unordered_map>
#include <functional>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Context {
    std::function<void(const int sockId, const std::function<void(std::string b)> x)> consumer;
    std::function<void(const int sockId, const std::function<const std::string()> x)> sender;
    std::unordered_map<std::string, int> connectedAdmins;
    std::unordered_map<std::string, int> connectedPlayers;
    std::unordered_map<std::string, int> connectedTrainers;
};

struct Request {
    int sockId;
    int sockRole;
    std::string sockName;

    std::string action;
    json data;
};

struct Response {
    std::string action;
    std::string message;
    json data;
};

class Server {
    protected:
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

        virtual void addClient(const std::string name, const int sockId) = 0;

        void registerEndpoint(std::string action, std::function<Response(Request request)> handler) {
            _endpoints.insert({action, handler});
        }

        Response subscribe(Request request) {
            return executeEndpoint(request.action, request);
        }
};

#endif
