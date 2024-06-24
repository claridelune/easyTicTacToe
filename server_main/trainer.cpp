#include "trainer.hpp"
#include <iostream>

std::string TrainerServer::emit() {
    return "data";
}

void TrainerServer::subscribe(std::string data) {
    std::cout << data << std::endl;
}

void TrainerServer::broadcast(std::string payload) {
    std::cout << payload << std::endl;
}