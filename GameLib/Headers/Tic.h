#include <iostream>
#include <windows.h>
#include <tuple>
#include <map>
#include "Color.h"

typedef std::tuple<int, int> Cord;

class Tic
{
private:
    char board[3][3];
    int game, turn, boardCount;
    int row, col;
    char human, ai, winner;
    std::map<char, int> scoreMap;
    void draw(int end);
    void takeInput();
    void aiMove();
    char getResult();
    Cord findMove();
    int minimax(int depth, bool isMaximizing);
    void endGame();
public:
    Tic(char comp, char player, int first);
    void run();
};

Tic::Tic(char comp, char player, int first)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = '_';
        }
    }
    scoreMap.insert(std::pair<char, int>(comp, 1));
    scoreMap.insert(std::pair<char, int>(player, -1));
    scoreMap.insert(std::pair<char, int>('D', 0));
    human = player;
    ai = comp;
    turn = first;
    winner = 'N';
    boardCount = 0;
    row = 0;
    col = 0;
    game = 1;
}

void Tic::draw(int end) {
    system("cls");
    std::cout << Color(14) << "< < < TIC-TAC-TOE > > >" << Color(7) << std::endl;
    if (!end) {
        std::cout << "\nTurn: " << Color(5) << (turn ? "Computer\n" : "YOU\n") << Color(7) << std::endl;
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == row && j == col) {
                std::cout << Color(4) << (board[i][j] == '_' ? '#' : board[i][j]) << " " << Color(7);
            }
            else {
                std::cout << (board[i][j] == ai ? Color(3) : (board[i][j] == human ? Color(6) : Color(7))) << board[i][j] << " " << Color(7);
            }
        }
        std::cout << std::endl;
    }
    if (end) {
        endGame();
        game = 0;
    }
}

void Tic::endGame() {
    std::cout << Color(4) << "\nGAME ENDED!" << Color(7) << std::endl;
    if (winner != 'D') {
        std::cout << Color(2) << (winner == ai ? "Computer is" : "You are") << " the winner!" << Color(7) << std::endl;
    }
    else {
        std::cout << Color(3) << "ITS A DRAW!!!" << Color(7) << std::endl;
    }
    std::cout << "Press any key to exit . . ." << std::endl;
    std::cin.get();
}

void Tic::takeInput() {
    int ch = _getch();
    switch (ch) {
    case 72:
        //up
        row = (row == 0 ? 2 : row - 1);
        break;
    case 75:
        //left
        col = (col == 0 ? 2 : col - 1);
        break;
    case 77:
        //right
        col = (col == 2 ? 0 : col + 1);
        break;
    case 80:
        //down
        row = (row == 2 ? 0 : row + 1);
        break;
    case 13:
        //enter
        if (board[row][col] == '_') {
            board[row][col] = human;
            boardCount++;   //  total elements
            winner = getResult();
            if (winner != 'N') {
                draw(1);
            }
            turn = 1;   //  toggle turn
        }
        break;
    }
}

int Tic::minimax(int depth, bool isMaximizing) {
    char result = getResult();
    if (result != 'N') {
        return scoreMap[result];
    }
    if (isMaximizing) {
        int bestScore = -100;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == '_') {
                    board[i][j] = ai;
                    boardCount++;
                    int score = minimax(depth + 1, false);
                    board[i][j] = '_';
                    boardCount--;
                    bestScore = max(score, bestScore);
                }
            }
        }
        return bestScore;
    }
    else {
        int bestScore = 100;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == '_') {
                    board[i][j] = human;
                    boardCount++;
                    int score = minimax(depth + 1, true);
                    board[i][j] = '_';
                    boardCount--;
                    bestScore = min(score, bestScore);
                }
            }
        }
        return bestScore;
    }
}

Cord Tic::findMove() {
    Cord bestMove;
    int bestScore = -100;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '_') {
                board[i][j] = ai;
                boardCount++;
                int score = minimax(0, false);
                board[i][j] = '_';
                boardCount--;
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = std::make_tuple(i, j);
                }
            }
        }
    }
    return bestMove;
}

void Tic::aiMove() {
    Cord bestMove = findMove();
    int i = std::get<0>(bestMove);
    int j = std::get<1>(bestMove);
    board[i][j] = ai;
    boardCount++;   //  total elements
    winner = getResult();
    if (winner != 'N') {
        draw(1);
    }
    turn = 0;   //  toggle turn
}

char Tic::getResult() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] != '_')
                return board[i][0];
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] != '_')
                return board[0][i];
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[1][1] != '_')
            return board[1][1];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[1][1] != '_')
            return board[1][1];
    }
    if (boardCount == 9) {
        return 'D';
    }
    return 'N';
}

void Tic::run() {
    while (1) {
        draw(0);
        if (turn == 0) {
            takeInput();
            if (!game) {
                break;
            }
        }
        else {
            aiMove();
            if (!game) {
                break;
            }
        }
    }
}
