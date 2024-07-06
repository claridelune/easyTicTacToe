#include "admin.hpp"

#include <iostream>

void AdminServer::configure() {
    registerEndpoint("start", std::bind(&AdminServer::start, this, std::placeholders::_1));
    registerEndpoint("next", std::bind(&AdminServer::next, this, std::placeholders::_1));
}

Response AdminServer::start(Request request) {
    Response response;

    for(const auto& item : context().connectedTrainers) {
        context().sender(item.second, [&]() -> std::string {
            std::cout << "estoy enviando" << std::endl;

            return "abcd";
        });

        context().consumer(item.second, [&](char* buffer) {
            std::cout << "estoy recbiendo" << std::endl;
        });
    }

    response.action = request.action;
    response.message = "Data was Start correctly.";

    return response;
}

Response AdminServer::next(Request request) {
    Response response;

    response.action = request.action;
    response.message = "Data was Next correctly.";

    return response;
}