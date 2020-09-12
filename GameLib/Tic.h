#pragma once
#include <iostream>

class Tic
{
private:
	const int cells = 3;
	int board[3][3];
	int boardCount=0;
	int turn;
	int win;
	void toggleTurn();
	void getWinner();
	void endGame();
public:
	Tic() {
		for (int i = 0; i < cells; i++) {
			for (int j = 0; j < cells; j++) {
				board[i][j] = -1;
			}
		}
		turn = 0;
		win = -1;
	}
	void draw(int end);
	void playChance(int *game);
};

//	Draw the board and the stats
void Tic::draw(int end) {
	system("cls");
	std::cout << "TURN:\tPlayer " << turn+1 << std::endl;
	for (int i = 0; i < cells; i++) {
		for (int j = 0; j < cells; j++) {
			if (board[i][j] == 1) {
				std::cout << "X";
			}
			else if (board[i][j] == 0) {
				std::cout << "O";
			}
			else {
				std::cout << "-";
			}
		}
		std::cout << std::endl;
	}
	if (end) {
		endGame();
	}
}

//	Input values for turn
void Tic::playChance(int *game) {
	int r,c;
	while (1) {
		std::cout << "Type in the row and the column of the cell you want to use." << std::endl;
		std::cout << "Enter the row:\n";
		std::cin >> r;
		std::cout << "Enter the column:\n";
		std::cin >> c;
		if (r > 0 && r < 4 && c > 0 && r < 4) {
			if (board[r - 1][c - 1] == -1) {
				board[r - 1][c - 1] = turn;
				boardCount++;
				toggleTurn();
				getWinner();
				if (win != -1 || boardCount == cells*cells) {
					*game = 0;
					draw(1);
				}
				break;
			}
			else {
				std::cout << "The cell is already filled!" << std::endl;
			}
		}
		else {
			std::cout << "Please enter a valid cell row and column" << std::endl;
		}
	}
}

//	Toggle turn
void Tic::toggleTurn() {
	turn = !turn;
}

//	End Game
void Tic::endGame() {
	std::cout << "GAME OVER!" << std::endl;
	if (win == -1) {
		std::cout << "DRAW!!!" << std::endl;
	}
	else {
		std::cout << "PLAYER " << win+1 << " Wins!!!" << std::endl;
	}
}

//	Get the winner
void Tic::getWinner() {
	int midElement = board[1][1];
	if (midElement == board[0][0] && midElement == board[2][2]) {
		win = midElement;
	}
	else if (midElement == board[0][2] && midElement == board[2][0]) {
		win = midElement;
	}
	else if (midElement == board[0][1] && midElement == board[2][1]) {
		win = midElement;
	}
	else if (midElement == board[1][0] && midElement == board[1][2]) {
		win = midElement;
	}
	else if (board[0][0] == board[0][1] && board[0][0] == board[0][2]) {
		win = board[0][0];
	}
	else if (board[0][0] == board[1][0] && board[0][0] == board[2][0]) {
		win = board[0][0];
	}
	else if (board[2][2] == board[2][1] && board[2][2] == board[2][0]) {
		win = board[2][2];
	}
	else if (board[2][2] == board[1][2] && board[2][2] == board[0][2]) {
		win = board[2][2];
	}
	else {
		win = -1;
	}
}