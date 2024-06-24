#ifndef SERVER_MAIN_ADMIN_SERVER_H
#define SERVER_MAIN_ADMIN_SERVER_H

#include "server.hpp"
#include "admin.hpp"

class AdminServer : public Server { 
    public:
        std::string emit() override;
        void subscribe(std::string data) override;

        void broadcast(std::string payload) override;
};

#endif