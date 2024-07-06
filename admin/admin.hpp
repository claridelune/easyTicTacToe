#ifndef ADMIN_CLIENT_H
#define ADMIN_CLIENT_H

#include "../shared/client.hpp"
#include "../shared/utils.hpp"

class AdminClient : public Client {
    public:
        AdminClient(const std::string& ip, const size_t port, Logger* logger) : Client(ip, port, logger) {
            initialize();
        }

        void configure() override;
        void loop();
        void onStartResponse(Response response);
        void onNextResponse(Response response);
};

#endif
