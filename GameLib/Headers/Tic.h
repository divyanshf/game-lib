#include <iostream>
#include <windows.h>
#include <tuple>
#include <map>
#include <string>
#include "Color.h"

typedef std::tuple<int, int> Cord;
typedef std::tuple<int, int, int> Opt;

//  class tic
class Tic
{
private:
    char board[3][3];
    int nPlayers;
    int game, turn, boardCount;
    int row, col;
    char winner;
    char p1, p2;
    std::map<char, int> scoreMap;
    void draw(int end);
    void takeInput();
    void aiMove();
    char getResult();
    Cord findMove();
    int minimax(int depth, bool isMaximizing);
    void endGame();
public:
    Tic(char comp, char player, int first, int players);
    void run();
};

//  constructor
Tic::Tic(char player1, char player2, int first, int players)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = '_';
        }
    }
    nPlayers = players;
    p1 = player1;   //  p1 will be the ai player if players = 1
    p2 = player2;
    scoreMap.insert(std::pair<char, int>(p1, 1)); // ai map
    scoreMap.insert(std::pair<char, int>(p2, -1));
    scoreMap.insert(std::pair<char, int>('D', 0));
    turn = first;
    winner = 'N';
    boardCount = 0;
    row = 0;
    col = 0;
    game = 1;
}

//  draw the screen
void Tic::draw(int end) {
    system("cls");
    std::cout << Color(14) << "< < < TIC-TAC-TOE > > >\n" << Color(7) << std::endl;
    if (!end) {
        if (nPlayers == 1) {
            std::cout << "Turn: " << Color(5) << (turn == 0 ? "Computer\n" : "YOU\n") << Color(7) << std::endl;
        }
        else {
            std::cout << "Turn: " << Color(5) << (turn == 0 ? "Player 1\n" : "Player 2\n") << Color(7) << std::endl;
        }
    }
    else {
        row = -1;
        col = -1;
    }
    for (int i = 0; i < 3; i++) {
        std::cout << "\t";
        for (int j = 0; j < 3; j++) {
            if (i == row && j == col) {
                std::cout << Color(4) << (board[i][j] == '_' ? '#' : board[i][j]) << " " << Color(7);
            }
            else {
                std::cout << (board[i][j] == p1 ? Color(3) : (board[i][j] == p2? Color(6) : Color(7))) << board[i][j] << " " << Color(7);
            }
        }
        std::cout << std::endl;
    }
    if (end) {
        endGame();
        game = 0;
    }
}


//  end the game
void Tic::endGame() {
    std::cout << Color(4) << "\nGAME ENDED!" << Color(7) << std::endl;
    if (winner != 'D') {
        if (nPlayers == 1) {
            std::cout << Color(2) << (winner == p1 ? "Computer is" : "You are") << " the winner!" << Color(7) << std::endl;
        }
        else {
            std::cout << Color(2) << "Player " << (winner == p1 ? 1 : 2) << " is the winner!" << Color(7) << std::endl;
        }
    }
    else {
        std::cout << Color(3) << "ITS A DRAW!!!" << Color(7) << std::endl;
    }
    std::cout << "Press any key to exit . . ." << std::endl;
    std::cin.get();
}


//  take user input 
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
            board[row][col] = (turn == 0 ? p1 : p2);
            boardCount++;   //  total elements
            winner = getResult();
            if (winner != 'N') {
                draw(1);
            }
            turn = !turn;   //  toggle turn
        }
        break;
    }
}


//  minimax algorithm
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
                    board[i][j] = p1;
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
                    board[i][j] = p2;
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

//  find the best move
Cord Tic::findMove() {
    Cord bestMove;
    int bestScore = -100;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '_') {
                board[i][j] = p1;
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


//  AI's move
void Tic::aiMove() {
    Cord bestMove = findMove();
    int i = std::get<0>(bestMove);
    int j = std::get<1>(bestMove);
    board[i][j] = p1;
    boardCount++;   //  total elements
    winner = getResult();
    if (winner != 'N') {
        draw(1);
    }
    turn = !turn;   //  toggle turn
}

//  Get the winner of the game
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


//  Run the tic tac toe
void Tic::run() {
    if (nPlayers == 1) {
        while (1) {
            draw(0);
            if (turn == 0) {
                aiMove();
                if (!game) {
                    break;
                }
            }
            else {
                takeInput();
                if (!game) {
                    break;
                }
            }
        }
    }
    else {
        while (1) {
            draw(0);
            takeInput();
            if (!game) {
                break;
            }
        }
    }
}



// --------------------------------------------------------------------------------------------- //
//  Tic tac toe menu class



//  tic tac toe menu
class TicMenu {
private:
    std::map <int, std::string> nPlayerOptions;
    std::map <int, char> charOpt;
    int process1, process2, process3;
    int selectN;
    int selectC;
    int turn;
    void drawNPlayers();
    void inputNPlayers();
    void drawSelectChar();
    void inputChar();
    void drawTurn();
    void inputTurn();
public:
    TicMenu();
    Opt run();
};

//  menu constructor
TicMenu::TicMenu() {
    nPlayerOptions.insert(std::pair<int, std::string>(1, "You vs Computer"));
    nPlayerOptions.insert(std::pair<int, std::string> (2, "Player 1 vs Player 2"));
    charOpt.insert(std::pair<int, char>(0, 'O'));
    charOpt.insert(std::pair<int, char>(1, 'X'));
    process1 = 1;
    process2 = 1;
    process3 = 1;
    selectN = 1;
    selectC = 0;
    turn = 0;
}

//  draw the menu for number of players
void TicMenu::drawNPlayers() {
    system("cls");
    std::cout << Color(14) << "< < < TIC-TAC-TOE > > >\n" << Color(7) << std::endl;
    std::cout << "How many players ?\n" << std::endl;
    for (auto i = nPlayerOptions.begin(); i != nPlayerOptions.end(); i++) {
        std::cout << (selectN == (i->first) ? Color(2) : Color(7)) << (selectN == (i->first) ? "->" : "  ") << (i->second) << Color(7) << std::endl;
    }
    std::cout << std::endl;
}

//  input the number of players
void TicMenu::inputNPlayers() {
    int ch = _getch();
    switch (ch) {
    case 72:
        selectN = (selectN == 1 ? 2 : selectN - 1);
        break;
    case 80:
        selectN = (selectN == 2 ? 1 : selectN + 1);
        break;
    case 13:
        process1 = 0;
        break;
    }
}

//  draw the menu for the character selection
void TicMenu::drawSelectChar() {
    system("cls");
    std::cout << Color(14) << "< < < TIC-TAC-TOE > > >\n" << Color(7) << std::endl;
    std::cout << "What is the first player's (Computer for single player) move ?\n" << std::endl;
    for (auto i = charOpt.begin(); i != charOpt.end(); i++) {
        std::cout << (selectC == (i->first) ? Color(2) : Color(7)) << (selectC == (i->first) ? "->" : "  ") << (i->second) << Color(7) << std::endl;
    }
    std::cout << std::endl;
}

//  input the character of player
void TicMenu::inputChar() {
    int ch = _getch();
    switch (ch) {
    case 72:
        selectC = (selectC == 0 ? 1 : selectC - 1);
        break;
    case 80:
        selectC = (selectC == 1 ? 0 : selectC + 1);
        break;
    case 13:
        process2 = 0;
        break;
    }
}

//  draw the menu for first turn
void TicMenu::drawTurn() {
    system("cls");
    std::cout << Color(14) << "< < < TIC-TAC-TOE > > >\n" << Color(7) << std::endl;
    std::cout << "Who goes first ?\n" << std::endl;
    for (int i = 0; i < 2; i++) {
        std::cout << (turn == i ? Color(2) : Color(7));
        if (i == 0) {
            std::cout << (selectN == 1 ? "Computer" : "Player 1") << std::endl;
        }
        else {
            std::cout << (selectN == 1 ? "You" : "Player 2") << std::endl;
        }
    }
    std::cout << Color(7) << std::endl;
}

//  input the turn of players
void TicMenu::inputTurn() {
    int ch = _getch();
    if (ch == 72 || ch == 80) {
        turn = !turn;
    }
    if (ch == 13) {
        process3 = 0;
    }
}

//  the run function for tic tac toe menu
Opt TicMenu::run() {
    while (process1) {
        drawNPlayers();
        inputNPlayers();
    }
    while (process2) {
        drawSelectChar();
        inputChar();
    }
    while (process3) {
        drawTurn();
        inputTurn();
    }
    return Opt(selectN, selectC, turn);
}