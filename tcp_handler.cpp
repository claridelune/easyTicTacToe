#include "common.h"
#include "game.h"

#define PORT 8080

struct Cli {
  int sockfd;
  char buffer[BUFFER_SIZE];
  Game* game;

  Cli(int _socket) {
    game = new Game('X');
    sockfd = _socket;
  }
};

void read_data(int sockfd, char *buffer, int num_bytes) {
    if (recv(sockfd, buffer, num_bytes, 0) == -1) {
        perror("Error: receive failed");
        exit(EXIT_FAILURE);
    }
    buffer[num_bytes] = '\0';
}

void send_data(int sockfd, char *buffer, int num_bytes) {
  if(send(sockfd, buffer, num_bytes, 0) == -1) {
    std::cout << "Error: send client" << std::endl;
  }
}

void send_movement(Cli* cli) {
  // predicir movimiento con la red neuronal
  strcpy(cli->buffer, "R1");
  send_data(cli->sockfd, cli->buffer, 2);
}

void startGame(Cli *cli) {
  // S | firstplayer
  // generar tablero
  read_data(cli->sockfd, cli->buffer, 1);
  std::cout << "Starting game" << std::endl;
  cli->game->resetBoard();
  if (cli->buffer[1] == '0') {
    send_movement(cli);
  }

}

void clientMove(Cli *cli) {
  // M | index
  read_data(cli->sockfd, cli->buffer, 1);
  int move = cli->buffer[1];
  std::cout << "The move is: " << move << std::endl;
  // falta logica del currentPlayer
  cli->game->makeMove(move);
  send_movement(cli);
}

std::unordered_map<char, void (*)(Cli *)> actionTable = {
  {'S', &startGame},
  {'M', &clientMove},
};


void handle_tcp_connection(int client_socket, int sockfd, struct sockaddr_in sockaddr) {
  Cli cli(client_socket);

  while (1) {
    read_data(cli.sockfd, cli.buffer, 1);

    char action = cli.buffer[0];

    auto it = actionTable.find(action);

    if (it != actionTable.end()) {
      (it->second)(&cli);
    }
  }
}

void start_server() {
  int sockfd, client_socket;
  struct sockaddr_in server_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockfd == -1) {
    perror("Error: cannot create socket");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(struct sockaddr_in));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == -1) {
    perror("Error: bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(sockfd, 10) == -1) {
    perror("Error: listen failed");
    exit(EXIT_FAILURE);
  }

  printf("Server started. Waiting for connections...\n");

  while (1) {
    client_socket = accept(sockfd, NULL, NULL);
    if (sockfd == -1) {
      perror("Error: accept failed");
    }

    printf("Client connected.\n");

    std::thread clientThread(handle_tcp_connection, client_socket, sockfd, server_addr);
  }
}

