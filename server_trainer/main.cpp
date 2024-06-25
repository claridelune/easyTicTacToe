#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h>

#define PORT 4490

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Error al crear el socket" << std::endl;
        return -1;
    }

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT); 

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error al conectar al servidor" << std::endl;
        close(sockfd);
        return -1;
    }

    unsigned int rol = 0x4;

    std::string message = R"(
        {
            "action":"join",
            "credential":"user03",
            "data":{
                "property1":"123",
                "property2":"abc"
            }
        }
    )";
    send(sockfd, message.c_str(), message.length(), 0);

    char buffer[4096] = {0};
    recv(sockfd, buffer, 4096, 0);

     std::cout << buffer << std::endl;

    close(sockfd);

    return 0;
}
