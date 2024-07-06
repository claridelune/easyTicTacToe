#include <iostream>
#include "trainer.hpp"
#include "../shared/utils.hpp"

#define UDP_PORT "3490"
#define TCP_CLIENT_PORT 4490
#define TCP_SERVER_PORT 5490
#define ROL 3

int main() {
    Logger logger("TrainerServer");
    try {
        TrainerClient trainer("127.0.0.1", TCP_CLIENT_PORT, &logger);

        Request requestJoin;
        requestJoin.action = "join";
        requestJoin.credential = {{"role", ROL}, {"name", uuid()}};
        requestJoin.data = {};

        trainer.sendRequest(requestJoin);
        trainer.listen();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
