#include "manager.hpp"
#include "logger.hpp"

#define PORT 4001

Logger logger("main");

int main() {
    ServerManager* manager = new ServerManager();
    manager->run(PORT, []() {
        logger.info("Server is running...");
    });

    delete manager;
    return 0;
}