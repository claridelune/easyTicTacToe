#include <iostream>
#include "player.hpp"
#include "../shared/utils.hpp"

#define PORT 4490
#define ROL 2

PlayerClient* player;

void showBoard(const std::string& board)
{
    char symb[3] = {' ', 'X', 'O'};

    std::cout << '\n';
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << symb[board[i * 3 + j] - '0'];
            if (j < 2) {
                std::cout << " | ";
            }
        }
        std::cout << '\n';
        if (i < 2) {
            std::cout << "---------\n";
        }
    }
}

bool gameFinished(const std::string& board, bool turn, int pos)
{
    int y = pos / 3;
    int x = pos % 3;

    bool out = false;
    bool tmp;

    tmp = true;
    for (int i = 0; i < 3; i++)
    {
        if (x == i)
            continue;
        tmp = tmp && board[y*3 + i] == '1' + turn;
    }
    out = out || tmp;

    tmp = true;
    for (int i = 0; i < 3; i++)
    {
        if (y == i)
            continue;
        tmp = tmp && board[i*3 + x] == '1' + turn;
    }
    out = out || tmp;

    tmp = true;
    for (int i = 0; i < 3; i++)
    {
        int j = i;
        tmp = tmp && ((i == y && j == x) || board[i*3 + j] == '1' + turn);
    }
    out = out || tmp;

    tmp = true;
    for (int i = 0; i < 3; i++)
    {
        int j = 2 - i;
        tmp = tmp && ((i == y && j == x) || board[i*3 + j] == '1' + turn);
    }
    out = out || tmp;

    return out;
}

int getPlayerMove(const std::string& board)
{
    int move = -1;
    std::cout << "Its your turn\n";
    while (move < 0 || move > 8 || board[move] != '0')
    {
        std::cout << "Move (0-8): ";
        std::cin >> move;
    }
    return move;
}

int getAIMove(const std::string& board)
{
    Request request;
    request.action = "predict";
    request.credential = {{"role", ROL}, {"name", uuid()}};
    request.data = {{"board", board.c_str()}};

    player->sendRequest(request);
    player->listen();
    return player->getLastMove();
}

void gameLoop()
{
    char ans = ' ';
    while (ans != 'Y' && ans != 'N' && ans != 'y' && ans != 'n')
    {
        std::cout << "Want to play first? (Y/N) ";
        std::cin >> ans;
    }

    bool turn = ans == 'N' || ans == 'n';

    int(*getMove[2])(const std::string&) = {getPlayerMove, getAIMove};

    std::string board = "000000000";

    int i = 0;
    for (; i < 9; i++)
    {
        showBoard(board);

        int pos = getMove[turn](board);

        board[pos] = turn + '1';

        if (gameFinished(board, turn, pos))
        {
            showBoard(board);
            std::cout << ((turn) ? "You Lose!\n" : "You win!\n");
            break;
        }

        turn = !turn;
    }

    if (i == 9)
    {
        showBoard(board);
        std::cout << "Draw!\n";
    }
}

int main()
{
    std::cout << "TIC TAC TOE\n";
    std::cout << "Connecting to server...\n";

    Logger logger("PlayerClient");
    player = new PlayerClient("127.0.0.1", PORT, &logger);

    Request requestJoin;
    requestJoin.action = "join";
    requestJoin.credential = {{"role", ROL}, {"name", uuid()}};
    requestJoin.data = {};

    player->sendRequest(requestJoin);
    player->listen();

    bool keep = true;
    while (keep)
    {
        gameLoop();
        char ans = ' ';
        while (ans != 'Y' && ans != 'N' && ans != 'y' && ans != 'n')
        {
            std::cout << "Want to play again? (Y/N) ";
            std::cin >> ans;
        }

        keep = ans == 'Y' || ans == 'y';
    }

    delete player;

    return 0;
}
