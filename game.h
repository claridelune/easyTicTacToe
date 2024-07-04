#include "common.h"

class Game {
public:
  Game(char cp) : currentPlayer(cp) {}

  // Reinicia el tablero
  void resetBoard() {
    for (int i = 0; i < 9; ++i) {
      board[i] =
          '1' + i; // Numeros del 1 al 9 para representar las casillas vacias
    }
  }

  // Realiza un movimiento en el tablero
  bool makeMove(int position) {
    if (position < 1 || position > 9 || board[position - 1] == 'X' ||
        board[position - 1] == 'O') {
      return false; // Movimiento inválido
    }
    board[position - 1] = currentPlayer;
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X'; // Cambia el jugador
    return true;                                        // Movimiento válido
  }

  // Verifica si hay un ganador o si hay empate
  char checkWinner() {
    // Combinaciones ganadoras
    const std::vector<std::vector<int>> winConditions = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Filas
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columnas
        {0, 4, 8}, {2, 4, 6}             // Diagonales
    };

    for (const auto& condition : winConditions) {
      if (board[condition[0]] == board[condition[1]] &&
          board[condition[1]] == board[condition[2]]) {
        return board[condition[0]]; // Devuelve el símbolo del ganador
      }
    }

    // Si no hay ganador y no quedan casillas vacías, es un empate
    for (char c : board) {
      if (c != 'X' && c != 'O')
        return '-'; // Aún hay casillas vacías
    }

    return ' '; // Empate
  }

  std::string getBoard() {
    std::string boardString(board, 9);
    return boardString;
  }

  char currentPlayer; // Jugador actual ('X' o 'O')
private:
  char board[9]; // Tablero del juego
};
