#ifndef SERVER_MAIN_SERVER_H
#define SERVER_MAIN_SERVER_H

#include <functional>

class Server {
    private:
        // struct sockaddr_in _
        void loop();

    public:
        Server();
        ~Server();
        
        void listen(size_t port, std::function<void()> handler);

        void start();
        void next();
        void close();
};

#endif