#include "admin.hpp"

#include <iostream>

void AdminServer::configure() {
    registerEndpoint("start", std::bind(&AdminServer::start, this, std::placeholders::_1));
    registerEndpoint("next", std::bind(&AdminServer::next, this, std::placeholders::_1));
}

Response AdminServer::start(Request request) {
    Response response;

    Context& ctx = context();
    std::cout << "STARTTTTT addClient " << ctx.connectedAdmins.size() << std::endl;

    for(const auto& item : ctx.connectedAdmins) {
        std::cout << "senderrrrrrrrrrrrrrrrrrrrrrrrrr" << item.second << std::endl;
        ctx.sender(item.second, [&]() -> std::string {
            std::cout << "estoy enviando" << std::endl;

            return R"(
                {
                  "action": "next",
                  "message": "enviado desde un callback",
                  "data": {
                      "property": "abc",
                      "nani": "123"
                  }
                }
            )";
        });

        ctx.consumer(item.second, [&](char* buffer) {
            std::cout << "estoy recbiendo" << std::endl;
            std::cout << buffer << std::endl;
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