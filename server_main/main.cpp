#include "server.hpp"
#include "logger.hpp"

#define PORT 4000

Logger logger("main");

int main() {
    Server* server = new Server();
    server->listen(PORT, []() {
        logger.info("Server is running...");
    });

    delete server;
    return 0;
}