#include "player.hpp"

void PlayerClient::configure() {
    registerVoidHandler("join", std::bind(&PlayerClient::onJoinResponse, this, std::placeholders::_1));
    registerVoidHandler("predict", std::bind(&PlayerClient::onPredictResponse, this, std::placeholders::_1));
}

void PlayerClient::onJoinResponse(Response response) {
    std::cout << "Connected to server.\n";
}

void PlayerClient::onPredictResponse(Response response)
{
    lastMove = response.data["move"];
}

int PlayerClient::getLastMove()
{
    return lastMove;
}
