#include "admin.hpp"

#define ROL 1

void AdminClient::configure() {
    registerHandler("start", std::bind(&AdminClient::onStartResponse, this, std::placeholders::_1));
    registerHandler("next", std::bind(&AdminClient::onNextResponse, this, std::placeholders::_1));
}

void AdminClient::loop() {
    while (true) {
        Request request;
        Response response;

        request.credential = {{"role", ROL}, {"name", getIdentity()}};

        std::string input;
        std::cin >> input;

        if (input == "start") {
            request.action = "start";
        } else if (input == "continue") {
            request.action = "next";
        } else {
            logger->info("Opción no válida");
            continue;
        }

        sendRequest(request);
        listen();
    }
}

Request AdminClient::onStartResponse(Response response) {
    Request request;
    
    request.action = response.action;


    logger->info("Start Response: " + response.message);

    return request;
}

Request AdminClient::onNextResponse(Response response) {
    Request request;

    request.action = response.action;

    logger->info("Next Response: " + response.message);
    
    return request;
}