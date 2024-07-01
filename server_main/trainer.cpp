#include "trainer.hpp"

void TrainerServer::configure() {
    registerEndpoint("join", std::bind(&TrainerServer::join, this, std::placeholders::_1));
    registerEndpoint("load", std::bind(&TrainerServer::load, this, std::placeholders::_1));
}

Response TrainerServer::join(Request request) {
    Response response;

    // mocked data
    _config->set({"server1", "192.168.1.1", true});
    _config->set({"server2", "192.168.1.2", false});
    _config->set({"server3", "192.168.1.3", false});
    _config->set({"server4", "192.168.1.4", false});
    _config->set({"server5", "192.168.1.5", false});
    // end mocked

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

Response TrainerServer::load(Request request) {
    Response response;

    response.action = request.action;
    response.message = "Data was uploaded correctly.";

    return response;
}