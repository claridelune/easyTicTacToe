#ifndef SERVER_MAIN_ADMIN_SERVER_H
#define SERVER_MAIN_ADMIN_SERVER_H

#include "server.hpp"
#include "admin.hpp"

class AdminServer : public Server {
    private:
        void addClient(const std::string name, const int sockId) override {
            auto records = context().connectedAdmins;
            auto admin = records.find(name);
            if (admin == records.end())
                records.insert({ name, sockId });
        }

    public:
        AdminServer(Context& context): Server(context) { 
            initialize(); 
        }

        void configure() override;

        Response start(Request request);
        Response next(Request request);
};

#endif