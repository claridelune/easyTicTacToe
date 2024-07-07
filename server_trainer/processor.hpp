#ifndef SERVER_TRAINER_PROCESSOR_H
#define SERVER_TRAINER_PROCESSOR_H

#include <string>
#include <variant>
#include <functional>
#include <unordered_map>
#include <type_traits>

#include <nlohmann/json.hpp>

#include "../shared/logger.hpp"
#include "../shared/socket.hpp"

#define TRAINER_ROLE 3
#define RESPONSE_VOID "void"

using json = nlohmann::json;

struct Request {
    std::string action;
    json data;
};

struct Response {
       // int sockId;
    // int sockRole;
    // std::string sockName;
    
    std::string action;
    json credential;
    json data;
};

struct ProcessorOpts {
    std::string uid;
    std::string ipAddress;
    int port;
};

class TrainerProcessor {
    protected:
        Socket* _socket;
        ProcessorOpts _options;

        std::unordered_map<std::string, std::variant<std::function<void(Request)>, std::function<Response(Request)>>> _endpoints;

        void setOptions(ProcessorOpts opts) {
            _options = opts;
        }
        ProcessorOpts& getOptions() {
            return _options;
        }

        Response executeEndpoint(std::string action, Request request) {
            auto fn = _endpoints.find(action);
            if (fn != _endpoints.end()) {
                return std::visit([&request, &action](auto&& fn) -> Response {
                    using T = std::decay_t<decltype(fn)>;
                    if constexpr (std::is_same_v<T, std::function<void(Request)>>) {
                        fn(request);
                        return { RESPONSE_VOID, "Action completed.", json() };
                    } else if constexpr (std::is_same_v<T, std::function<Response(Request)>>) {
                        return fn(request);
                    }
                }, fn->second);
            }

            return {
                request.action,
                "Action not found.",
                json()
            };
        }

    public:
        TrainerProcessor(Socket* socket) : _socket(socket) {}

        virtual void initialize() = 0;
        virtual void configure() = 0;

        void registerEndpoint(const std::string action, std::function<Response(Request)> handler) {
            _endpoints.insert({action, handler});
        }

        void registerVoidEndpoint(const std::string action, std::function<void(Request)> handler) {
            _endpoints.insert({action, handler});
        }

        Response subscribe(Request request) {
            return executeEndpoint(request.action, request);
        }

        void send(Response response) {
            int sockId = _socket->getIdentity();
            _socket->sender(sockId, [&]() -> std::string {
                json jsonMeta = {
                    {"action", response.action},
                    {"credential", {
                        {"role", TRAINER_ROLE},
                        {"name", _options.uid}
                    }},
                    {"data", response.data}
                };
                
                std::string res = jsonMeta.dump();
                return res;
            });
        }

        Request receive() {
            int sockId = _socket->getIdentity();
            Request req;
            _socket->consumer(sockId, [&](std::string buffer) {
                json payload = json::parse(buffer);
                req.action = payload["action"];
                req.data = payload.value("data", json::object());
            });

            return req;
        }
};

#endif