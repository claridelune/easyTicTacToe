#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(0, 8);

void showBoard(const std::string& board)
{
    char symb[3] = {' ', 'O', 'X'};

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

std::string generateBoard()
{
    int numTurns = dis(gen);
    bool turn = !(numTurns & 1);

    std::string board;
    board.resize(9, '0');

    for (int i = 0; i < numTurns; )
    {
        int cnt = 0;
        for (int j = 0; j < 9; j++)
        {
            cnt += board[j] == '0' && !gameFinished(board, turn, j);
        }

        if (!cnt)
            return "";

        int pos = dis(gen);
        if (board[pos] == '0' && !gameFinished(board, turn, pos))
        {
            board[pos] = '1' + turn;
            turn = !turn;
            i++;
        }
    }

    return board;
}

char getLabel(const std::string& board)
{
    char out;
    showBoard(board);
    std::cout << "Best Move: ";
    std::cin >> out;
    return out;
}

int main(int argc, char *argv[])
{
    int numDatos = (argc >= 2) ? std::stoi(argv[1]) : 10;
    std::string filename = (argc >= 3) ? argv[2] : "data.txt";

    std::cout << "Pass as arguments to the program the number of data to be generated and the filename where you want to store/append it.\n";
    std::cout << "Generating " << numDatos << " data in " << filename << ".\n";
    std::cout << "You are X. A move is an integer from 0 to 8.\n";

    auto lamb = [](const std::string& x, const std::string& y) {
        return x.substr(0, 9) < y.substr(0, 9);
    };
    std::set<std::string, decltype(lamb)> data(lamb);
    
    std::fstream dataFile;

    dataFile.open(filename, std::fstream::in);

    int initialSize = 0;

    if (dataFile)
    {
        std::string line;
        while (std::getline(dataFile, line))
        {
            data.insert(line);
        }
        initialSize = data.size();
    }

    dataFile.close();
    
    dataFile.open(filename, std::fstream::out | std::fstream::trunc);

    while (data.size() < initialSize + numDatos)
    {
        std::string board = generateBoard();
        if (board == "")
            continue;
        auto it = data.find(board);

        if (it == data.end())
        {
            char label = getLabel(board);
            board.push_back(label);
            data.insert(board);
        }
    }

    for (auto& dat : data)
    {
        dataFile << dat << '\n';
    }
    
    dataFile.close();

    return 0;
}