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
#include "../neural_network/neuralNetwork.hpp"
#include "../neural_network/dataHandler.hpp"

#define TRAINER_ROLE 3
#define RESPONSE_VOID "void"

using json = nlohmann::json;

struct Request {
    int sockId;
    std::string sockName;
    
    std::string action;
    json data;
};

struct Response {
    std::string action;
    std::string message;
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
        NeuralNetwork* _nn;
        DataHandler dataHandler;

        std::vector<std::vector<double>> train_data;
        std::vector<int> train_labels;

        std::unordered_map<std::string, std::variant<std::function<void(Request)>, std::function<Response(Request)>>> _endpoints;

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
        TrainerProcessor(Socket* socket, NeuralNetwork* nn) : _socket(socket), _nn(nn) {}

        virtual void initialize() = 0;
        virtual void configure() = 0;

        virtual void send(Response response, Socket* socket = nullptr) = 0;
        virtual Request receive(Socket* socket = nullptr) = 0;

        int getSockId() { return _socket->getIdentity(); }
        void setSockId(int sockId) { _socket->setIdentity(sockId); }
        void closeSock(int sockId) { _socket->close(sockId);  }

        void setOptions(ProcessorOpts opts) { _options = opts; }
        ProcessorOpts& getOptions() { return _options; }

        void registerEndpoint(const std::string action, std::function<Response(Request)> handler) {
            _endpoints.insert({action, handler});
        }

        void registerVoidEndpoint(const std::string action, std::function<void(Request)> handler) {
            _endpoints.insert({action, handler});
        }

        Response subscribe(Request request) {
            return executeEndpoint(request.action, request);
        }
};

#endif
