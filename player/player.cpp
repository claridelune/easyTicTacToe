#include "player.hpp"

void PlayerClient::configure() {
    registerHandler("join", std::bind(&PlayerClient::onJoinResponse, this, std::placeholders::_1));
}

void PlayerClient::onJoinResponse(Response response) {
    logger->info("Join Response: " + response.message);
}
