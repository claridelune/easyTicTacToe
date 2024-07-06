#include <iostream>
#include "player.hpp"
#include "../shared/utils.hpp"

#define PORT 4490
#define ROL 2

int main() {
    Logger logger("PlayerClient");
    try {
        PlayerClient player("127.0.0.1", PORT, &logger);

        Request requestJoin;
        requestJoin.action = "join";
        requestJoin.credential = {{"role", ROL}, {"name", uuid()}};
        requestJoin.data = {};

        player.sendRequest(requestJoin);
        player.listen();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
