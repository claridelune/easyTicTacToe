#include "player.hpp"
#include <iostream>

void PlayerServer::configure() {
    registerEndpoint("predict", std::bind(&PlayerServer::predict, this, std::placeholders::_1));
}

Response PlayerServer::predict(Request request) {
    Response response;
    Context& ctx = context();

    if (!ctx.connectedTrainers.empty()) {
        auto firstTrainer = ctx.connectedTrainers.begin();

        ctx.sender(firstTrainer->second, [&]() -> std::string {
            json request_json = {
                {"action", request.action},
                {"data", request.data}
            };
            return request_json.dump();
        });

        std::string trainer_response;

        ctx.consumer(firstTrainer->second, [&](std::string buffer) {
            std::cout << buffer << std::endl;
            trainer_response = buffer;
        });

        if (!trainer_response.empty()) {
            json trainer_json = json::parse(trainer_response);
            response.action = trainer_json["action"].get<std::string>();
            response.data = trainer_json["data"];
        } else {
            response.action = request.action;
            response.data = {{"error", "No response from trainer"}};
        }
    } else {
        response.action = request.action;
        response.message = "error: No connected trainers";
        return response;
    }

    response.message = "Mi prediccion nunca falla >:V";
    response.data = {{"move", 4}};

    return response;
}
