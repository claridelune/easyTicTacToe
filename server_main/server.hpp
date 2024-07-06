#ifndef SERVER_MAIN_SERVER_H
#define SERVER_MAIN_SERVER_H

#include <string>
#include <unordered_map>
#include <functional>

#include <nlohmann/json.hpp>

// #include "manager.hpp"

using json = nlohmann::json;

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
        // Context* _context;

        std::unordered_map<std::string, int> _connectedClients;
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

        void addClient(const std::string name, const int sockId) {
            auto client = _connectedClients.find(name);
            if (client == _connectedClients.end())
                _connectedClients.insert({ name, sockId });
        }

        // void setContext(Context* context) {
        //     if (_context == nullptr)
        //         _context = context;
        // }

        // Context* context() {
        //     return _context;
        // }

    public:
        Server() { }

        virtual void configure() = 0;

        void registerEndpoint(std::string action, std::function<Response(Request request)> handler) {
            _endpoints.insert({action, handler});
        }

        Response subscribe(Request request) {
            // setContext(context);
            addClient(request.sockName, request.sockId);
            return executeEndpoint(request.action, request);
        }

        const std::unordered_map<std::string, int>& getConnected() const {
            return _connectedClients;
        }
};

#endif