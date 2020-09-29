#include "TicBoard.h"

TicBoard::TicBoard(const char* p1, const char* p2) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = " ";
        }
    }
    boardCount = 0;
    scoreMap.insert(std::pair<const char*, int>(p1, 1)); // ai map
    scoreMap.insert(std::pair<const char*, int>(p2, -1));
    scoreMap.insert(std::pair<const char*, int>("D", 0));
    turn = (rand() % 2) < 0.5 ? 0 : 1;
    winner = "N";
    player1 = p1;
    player2 = p2;
}

int TicBoard::humanMove(int row, int col) {
    if (board[row][col] == " ") {
        board[row][col] = (turn == 0 ? player1 : player2);
        boardCount++;   //  total elements
        winner = getResult();
        if (winner != "N") {
            return 1;
        }
        turn = !turn;   //  toggle turn
    }
    return 0;
}
Cord TicBoard::findMove() {
    Cord bestMove;
    int bestScore = -100;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == " ") {
                board[i][j] = player1;
                boardCount++;
                int score = minimax(0, false);
                board[i][j] = " ";
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
int TicBoard::minimax(int depth, bool isMaximizing) {
    const char* result = getResult();
    if (result != "N") {
        return scoreMap[result];
    }
    if (isMaximizing) {
        int bestScore = -100;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == " ") {
                    board[i][j] = player1;
                    boardCount++;
                    int score = minimax(depth + 1, false);
                    board[i][j] = " ";
                    boardCount--;
                    bestScore = std::max(score, bestScore);
                }
            }
        }
        return bestScore;
    }
    else {
        int bestScore = 100;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == " ") {
                    board[i][j] = player2;
                    boardCount++;
                    int score = minimax(depth + 1, true);
                    board[i][j] = " ";
                    boardCount--;
                    bestScore = std::min(score, bestScore);
                }
            }
        }
        return bestScore;
    }
}
int TicBoard::aiMove() {
    Cord bestMove = findMove();
    int i = std::get<0>(bestMove);
    int j = std::get<1>(bestMove);
    board[i][j] = player1;
    boardCount++;   //  total elements
    winner = getResult();
    if (winner != "N") {
        return 1;
    }
    turn = !turn;   //  toggle turn
    return 0;
}
const char* TicBoard::getResult() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] != " ")
                return board[i][0];
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] != " ")
                return board[0][i];
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[1][1] != " ")
            return board[1][1];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[1][1] != " ")
            return board[1][1];
    }
    if (boardCount == 9) {
        return "D";
    }
    return "N";
}

