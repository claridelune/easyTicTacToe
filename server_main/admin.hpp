#ifndef SERVER_MAIN_ADMIN_SERVER_H
#define SERVER_MAIN_ADMIN_SERVER_H

#include "server.hpp"
#include "admin.hpp"

#include <iostream>

class AdminServer : public Server {
    private:
        Context& _context;

        Context& context() {
            return _context;
        }

    public:
        AdminServer(Context& context): _context(context) { 
            initialize(); 
        }

        void configure() override;

        void addClient(const std::string name, const int sockId) override {
            Context& ctx = context();
            ctx.connectedAdmins[name] = sockId;
        }

        Response start(Request request);
        Response next(Request request);
};

#endif