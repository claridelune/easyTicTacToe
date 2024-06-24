#ifndef SERVER_MAIN_TRAINER_SERVER_H
#define SERVER_MAIN_TRAINER_SERVER_H

#include "server.hpp"
#include "trainer.hpp"

class TrainerServer : public Server { 
    public:
        std::string emit() override;
        void subscribe(std::string data) override;

        void broadcast(std::string payload) override;
};

#endif