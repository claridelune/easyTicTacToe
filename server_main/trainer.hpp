#ifndef SERVER_MAIN_TRAINER_SERVER_H
#define SERVER_MAIN_TRAINER_SERVER_H

#include "server.hpp"
#include "trainer.hpp"
#include "config.hpp"

class TrainerServer : public Server { 
    private:
        Config* _config;

    public:
        TrainerServer() { 
            initialize();
            _config = new Config();
        }

        ~TrainerServer() {
            delete _config;
        }

        void configure() override;

        Response join(Request request);
        Response load(Request request);
};

#endif