#ifndef SERVER_MAIN_SERVER_H
#define SERVER_MAIN_SERVER_H

#include <string>

class Server {
    public:
        virtual ~Server() {}
        virtual std::string emit() = 0;
        virtual void subscribe(std::string data) = 0;

        virtual void broadcast(std::string payload) = 0;
};

#endif