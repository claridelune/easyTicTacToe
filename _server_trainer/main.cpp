#include "trainerClient.hpp"

#define TCP_CLIENT_PORT 4490

int main() {
    Logger logger("TrainerServer");
    TrainerClient trainer("127.0.0.1", TCP_CLIENT_PORT, &logger);
    trainer.loop();
}