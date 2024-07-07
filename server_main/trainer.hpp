#ifndef SERVER_MAIN_TRAINER_SERVER_H
#define SERVER_MAIN_TRAINER_SERVER_H

#include <thread>
#include <unistd.h>
#include "server.hpp"
#include "trainer.hpp"
#include "config.hpp"
#include <iostream>

class TrainerServer : public Server { 
    private:
        Config* _config;

        Context& _context;

        bool _isRunning;
        std::unordered_map<std::string, std::chrono::steady_clock::time_point> _lastKeepAliveResponse;

        Context& context() {
            return _context;
        }

        void keepAlive();
        void reassignLeader();
        void sendConfiguration();

    public:
        TrainerServer(Context& context) : _context(context) { 
            initialize();
            _config = new Config();
            _isRunning = true;
        }

        ~TrainerServer() {
            delete _config;
            _isRunning = false;
        }

        void configure() override;

        void addClient(const std::string name, const int sockId) override {
            Context& ctx = context();
            ctx.connectedTrainers[name] = sockId;
        }
        void run();
        void stop();

        Response join(Request request);
        Response predict(Request request);
        Response handleKeepAlive(Request request);
};

#endif
