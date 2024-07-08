#include "admin.hpp"

#include <iostream>

void AdminServer::configure() {
    registerEndpoint("start", std::bind(&AdminServer::start, this, std::placeholders::_1));
    registerEndpoint("next", std::bind(&AdminServer::next, this, std::placeholders::_1));
}

Response AdminServer::start(Request request) {
    Response response;

    Context& ctx = context();

    for(const auto& item : ctx.connectedTrainers) {
        ctx.sender(item.second, [&]() -> std::string {
            json response = {
                {"action", request.action}
            };
            std::string res = response.dump();
            return res;
        });
    }

    response.action = request.action;
    response.message = "Data was Start correctly.";

    return response;
}

Response AdminServer::next(Request request) {
    Response response;

    Context& ctx = context();

    for(const auto& item : ctx.connectedTrainers) {
        ctx.sender(item.second, [&]() -> std::string {
            json response = {
                {"action", "start"}
            };
            std::string res = response.dump();
            return res;
        });
    }

    response.action = request.action;
    response.message = "Data was Next correctly.";

    return response;
}
