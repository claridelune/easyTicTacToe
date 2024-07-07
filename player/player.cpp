#include "player.hpp"

void PlayerClient::configure() {
    registerHandler("join", std::bind(&PlayerClient::onJoinResponse, this, std::placeholders::_1));
}

Request PlayerClient::onJoinResponse(Response response) {
    Request request;
    
    request.action = response.action;
    
    logger->info("Join Response: " + response.message);

    return request;
}
