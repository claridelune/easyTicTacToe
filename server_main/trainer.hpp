#ifndef SERVER_MAIN_TRAINER_SERVER_H
#define SERVER_MAIN_TRAINER_SERVER_H

#include "server.hpp"
#include "trainer.hpp"
#include "config.hpp"

class TrainerServer : public Server { 
    private:
        Config* _config;

        void addClient(const std::string name, const int sockId) override {
            auto records = context().connectedTrainers;
            auto trainer = records.find(name);
            if (trainer == records.end())
                records.insert({ name, sockId });
        }

    public:
        TrainerServer(Context& context): Server(context) { 
            initialize();
            _config = new Config();
        }

        ~TrainerServer() {
            delete _config;
        }

        void configure() override;

        Response join(Request request);
        Response predict(Request request);
};

#endif