#ifndef SERVER_MAIN_ADMIN_SERVER_H
#define SERVER_MAIN_ADMIN_SERVER_H

#include "server.hpp"
#include "admin.hpp"

class AdminServer : public Server { 
    public:
        AdminServer() { initialize(); }

        void configure() override;
};

#endif