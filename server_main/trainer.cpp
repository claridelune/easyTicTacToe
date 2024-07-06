#include "trainer.hpp"

void TrainerServer::configure() {
    registerEndpoint("join", std::bind(&TrainerServer::join, this, std::placeholders::_1));
    registerEndpoint("predict", std::bind(&TrainerServer::predict, this, std::placeholders::_1));
}

Response TrainerServer::join(Request request) {
    Response response;

    _config->set({request.sockName, "0.0.0.0", false});

    response.action = request.action;
    response.message = "Data has been successfully propagated.";

    for (const auto& item : _config->all()) {
        json jsonArray = {
            {"identity", item.identity},
            {"ipAddress", item.ipAddress},
            {"isLeader", item.isLeader}
        };
        response.data.push_back(jsonArray);
    }

    return response;
}

Response TrainerServer::predict(Request request) {
    Response response;

    response.action = request.action;
    response.message = "Data was uploaded correctly.";

    return response;
}