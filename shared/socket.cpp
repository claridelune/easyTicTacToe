#include "socket.hpp"

Socket::Socket() {
    _logger = new Logger("Socket");
}

Socket::Socket(const size_t port) {
    _socketAddr.sin_family = AF_INET;
    _socketAddr.sin_port = htons(port); 
    _socketAddr.sin_addr.s_addr = INADDR_ANY; 

    _logger = new Logger("Socket(server)");
}

Socket::Socket(const std::string& ip, const size_t port) {
    _socketAddr.sin_family = AF_INET;
    _socketAddr.sin_port = htons(port); 
    _socketAddr.sin_addr.s_addr  = inet_addr(ip.c_str());

    _logger = new Logger("Socket(client)");
}

Socket::~Socket() {
    delete _logger;
}

void Socket::initialize(const size_t port, std::optional<std::string> ip) {
    in_addr_t ipAddress = ip.has_value() ? inet_addr(ip->c_str()) : INADDR_ANY;

    _socketAddr.sin_family = AF_INET;
    _socketAddr.sin_port = htons(port); 
    _socketAddr.sin_addr.s_addr = ipAddress; 
}

int Socket::getIdentity() {
    return _socketId;
}

sockaddr_in Socket::getAddress() {
    return _socketAddr;
}

void Socket::configureServer() {
    _socketId = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketId == SOCKET_ERROR)
        throw std::runtime_error("Error creating socket.");

    if(bind(_socketId, (struct sockaddr *)&_socketAddr, sizeof(_socketAddr)) == SOCKET_ERROR)
        throw std::runtime_error("Error binding socket to address.");

    if (listen(_socketId, 5) == SOCKET_ERROR)
        throw std::runtime_error("Error when trying to listen for connections.");
}

void Socket::configureClient() {
    _socketId = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketId == SOCKET_ERROR)
        throw std::runtime_error("Error creating socket.");
}

void Socket::connectToServer() {
    if (connect(_socketId, (struct sockaddr *)&_socketAddr, sizeof(_socketAddr)) == SOCKET_ERROR)
        throw std::runtime_error("Error connecting to server.");
}

int Socket::accept() {
    int clientSocket = socketAccept(_socketId, nullptr, nullptr);
    if(clientSocket == SOCKET_ERROR)
        throw std::runtime_error("Error accepting incoming connection.");

    return clientSocket;
}

int Socket::consumer(const int socketId, const std::function<void(std::string b)> handler) {
    char bufferSize[PROTOCOL_SIZE];
    int receivedSize = recv(socketId, bufferSize, PROTOCOL_SIZE, 0);

    if (receivedSize == SOCKET_ERROR || receivedSize == 0)
        return receivedSize;

    int contentSize = hexToInt(bufferSize);

    _logger->info("consumer->buffer(size): [hex]=" + std::string(bufferSize) + " [size]=" + std::to_string(contentSize));

    std::vector<char> contentBuffer(contentSize);
    
    int totalReceived = 0;
    while(totalReceived < contentSize) {
        int bytesReceived = recv(socketId, contentBuffer.data() + totalReceived, contentSize - totalReceived, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
            return bytesReceived;

        totalReceived += bytesReceived;
    }

    const std::string contentValue = std::string(contentBuffer.begin(), contentBuffer.end());

    _logger->info("sender->buffer(content): " + contentValue);

    handler(contentValue);

    return receivedSize;
}
        
void Socket::sender(const int socketId, const std::function<const std::string()> handler) {
    const std::string contentValue = handler();
    const std::string contentSize = intToHex(contentValue.size());
    const std::string contentBuffer = contentSize + contentValue;

    _logger->info("sender->buffer(size): [hex]=" + contentSize + " [size]=" + std::to_string(contentValue.size()));

    size_t totalSent = 0;
    size_t contentLength = contentBuffer.size();
    const char* data = contentBuffer.c_str();

    _logger->info("sender->buffer(content): " + std::string(data));

    while(totalSent < contentLength) {
        int bytesSent = send(socketId, data + totalSent, contentLength - totalSent, 0);
        if (bytesSent == SOCKET_ERROR) {
            break;
        }

        totalSent += bytesSent;
    }
}

int Socket::close(int socketId) {
    return socketClose(socketId);
}
