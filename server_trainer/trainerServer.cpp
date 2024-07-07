#include "trainerServer.hpp"
#include <nlohmann/json.hpp>

TrainerServer::TrainerServer(size_t port) : _port(port) {}

void TrainerServer::configure() {
    serverSocket = std::make_unique<Socket>(_port);
    serverSocket->configureServer();

    while (true) {
        int clientSocket = serverSocket->accept();
        clientSockets.push_back(clientSocket);

        serverSocket->consumer(clientSocket, [&](char* buffer) {
            auto payload = nlohmann::json::parse(buffer);
            if (payload["action"] == "weights") {
                auto weights = payload["data"].get<std::vector<float>>();
                handleWeights(weights);
            }
        });
    }
}

void TrainerServer::handleWeights(const std::vector<float>& weights) {
    // Process the weights
    // Example: calculating the average and sending back to clients

    std::vector<float> averagedWeights(weights.size(), 0.0f);
    // Assuming weights from all clients are aggregated here
    // Simple averaging logic for demonstration
    for (auto& weight : weights) {
        for (size_t i = 0; i < averagedWeights.size(); ++i) {
            averagedWeights[i] += weight;
        }
    }

    for (size_t i = 0; i < averagedWeights.size(); ++i) {
        averagedWeights[i] /= clientSockets.size();
    }

    // Send back the averaged weights to all clients
    nlohmann::json responsePayload;
    responsePayload["action"] = "weights";
    responsePayload["data"] = averagedWeights;

    for (int clientSocket : clientSockets) {
        serverSocket->sender(clientSocket, [&responsePayload]() -> std::string {
            return responsePayload.dump();
        });
    }
}

void TrainerServer::startTraining() {}
