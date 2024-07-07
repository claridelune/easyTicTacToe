#include "trainer.hpp"

void TrainerClient::configure() {
    registerVoidHandler("join", std::bind(&TrainerClient::onJoinResponse, this, std::placeholders::_1));
}

void TrainerClient::onJoinResponse(Response response) {
    std::cout << "Join Response: " << response.message << std::endl;
    for (const auto& item : response.data) {
        std::cout << "Server: " << item["identity"] << " IP: " << item["ipAddress"] << " Leader: " << item["isLeader"] << std::endl;
    }
}
