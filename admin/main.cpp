#include "admin.hpp"

#define PORT 4490

int main() {
    Logger logger("AdminClient");
    try {
        AdminClient admin("127.0.0.1", PORT, &logger);

        admin.loop();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
