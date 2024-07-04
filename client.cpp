#include "common.h"
#include "game.h"

#define PORT 8080


void read_data(char *buffer, int sockfd, int num_bytes){
  if (recv(sockfd, buffer, num_bytes, 0) == -1) {
    perror("Error receive failed");
  }
  buffer[num_bytes] = '\n';
}

void move(char * buffer, int sockfd) {
  read_data(buffer, sockfd, 1);
  std::cout << "move: " << buffer[0] << std::endl;
}

void gameOver(char *buffer, int sockfd) {
  read_data(buffer, sockfd, 1);
  std::cout << "move: " << buffer[0] << std::endl;
}

std::unordered_map<char, void (*)(char *, int)> actionTable = {
  {'R', &move},
  {'F', &gameOver},
};

int main () {
  int client_socket;
  struct sockaddr_in server_addr;
  char buffer[BUFFER_SIZE];
  Game game('O');

  client_socket = socket(AF_INET,  SOCK_STREAM, 0);
  /*validacion*/

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(PORT);
  
  if(connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))<0) {
    // error
  }

  std::cout << "Connected" << std::endl;

  while (1) {
    read_data(buffer, client_socket, 1);

    char action = buffer[0];

    auto it = actionTable.find(action);
    if (it != actionTable.end()) {

    }
  }

  return 0;
}
