#include "socket.hpp"

Socket::Socket(const size_t port) {
    _socketAddr.sin_family = AF_INET;
    _socketAddr.sin_port = htons(port); 
    _socketAddr.sin_addr.s_addr = INADDR_ANY; 
}

Socket::~Socket() {}

int Socket::getIdentity() {
    return _socketId;
}

sockaddr_in Socket::getAddress() {
    return _socketAddr;
}

void Socket::configure() {
    _socketId = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketId == SOCKET_ERROR)
        throw std::runtime_error("Error creating socket.");

    if(bind(_socketId, (struct sockaddr *)&_socketAddr, sizeof(_socketAddr)) == SOCKET_ERROR)
        throw std::runtime_error("Error binding socket to address.");

    if (listen(_socketId, 5) == SOCKET_ERROR)
        throw std::runtime_error("Error when trying to listen for connections.");
}

int Socket::accept() {
    int clientSocket = socketAccept(_socketId, nullptr, nullptr);
    if(clientSocket == SOCKET_ERROR)
        throw std::runtime_error("Error accepting incoming connection.");

    return clientSocket;
}

int Socket::consumer(const int socketId, const std::function<void(char* buffer)> handler) {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    int bytesReceived = recv(socketId, buffer, sizeof(buffer), 0);
    if (bytesReceived != SOCKET_ERROR || bytesReceived != 0)
        handler(buffer);

    return bytesReceived;
}
        
void Socket::sender(const int socketId, const std::function<const std::string()> handler) {
    const std::string bytesSent = handler();
    send(socketId, bytesSent.c_str(), bytesSent.size() + 1, 0);
}

int Socket::close(int socketId) {
    return socketClose(socketId);
}