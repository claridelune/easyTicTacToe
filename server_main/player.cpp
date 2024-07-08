#include "player.hpp"
#include <iostream>

void PlayerServer::configure() {
    registerEndpoint("predict", std::bind(&PlayerServer::predict, this, std::placeholders::_1));
}

Response PlayerServer::predict(Request request) {
    Response response;

    response.action = request.action;
    response.message = "Mi prediccion nunca falla >:V";
    response.data = {{"move", 4}};

    return response;
}
