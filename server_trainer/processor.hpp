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

using json = nlohmann::json;

struct Request {
    int sockId;
    int sockRole;
    std::string sockName;
    
    std::string action;
    json credential;
    json data;
};

struct Response {
    std::string action;
    std::string message;
    json data;
};

struct ProcessorOpts {
    std::string ipAddress;
    int port;
};

class TrainerProcessor {
    protected:
        Socket* _socket;
        ProcessorOpts _options;

        std::unordered_map<std::string, std::variant<std::function<void(Request request)>, std::function<Response(Request request)>>> _endpoints;

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
                        return { action, "Action completed.", json() };
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
        void send(std::string b) {
            int sockId = _socket->getIdentity();
            _socket->sender(sockId, [&]() -> std::string {
                return b;
            });
        }
        std::string receive() {
            int sockId = _socket->getIdentity();

            std::string result;

            _socket->consumer(sockId, [&](std::string buffer) {
                result = buffer;
            });

            return result;
        }
};

#endif