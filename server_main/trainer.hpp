#ifndef SERVER_MAIN_TRAINER_SERVER_H
#define SERVER_MAIN_TRAINER_SERVER_H

#include "server.hpp"
#include "trainer.hpp"
#include "config.hpp"

class TrainerServer : public Server { 
    private:
        Config* _config;

        Context& _context;

        Context& context() {
            return _context;
        }

    public:
        TrainerServer(Context& context) : _context(context) { 
            initialize();
            _config = new Config();
        }

        ~TrainerServer() {
            delete _config;
        }

        void configure() override;

        void addClient(const std::string name, const int sockId) override {
            Context& ctx = context();
            ctx.connectedTrainers[name] = sockId;
        }

        Response join(Request request);
        Response predict(Request request);
};

#endif
