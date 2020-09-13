#pragma once
#include <iostream>
#include "Color.h"
#include <conio.h>

class Tic
{
private:
	const int cells = 3;
	int board[3][3];
	int boardCount=0;
	int turn, win;
	int row, col;
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
		row = 0, col = 0;
	}
	void draw(int end);
	void playChance(int* game);
	void input(int *game);
};

//	Draw the board and the stats
void Tic::draw(int end) {
	system("cls");
	std::cout << Color(14) << "< < < TIC-TAE-TOE > > >\n" << Color(7) << std::endl;
	if (!end)
		std::cout << "TURN:\n" << Color(2) << "Player" << turn + 1 << Color(7) << std::endl << std::endl;;
	for (int i = 0; i < cells; i++) {
		std::cout << "\t";
		for (int j = 0; j < cells; j++) {
			if (board[i][j] == 1) {
				std::cout << (row==i && col==j ? Color(4) : Color(6)) << "X " << Color(7);
			}
			else if (board[i][j] == 0) {
				std::cout << (row == i && col == j ? Color(4) : Color(9)) << "O " << Color(7);
			}
			else{
				std::cout << (row == i && col == j ? Color(4) : Color(7)) << (row == i && col == j ? "# " : "_ ") << Color(7);
			}
		}
		std::cout << std::endl << std::endl;
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

void Tic::input(int *game) {
	int ch = _getch();
	switch (ch) {
	case 72:
		//up
		row = (row== 0 ? 0 : row- 1);
		break;
	case 77:
		//right
		col = (col==cells-1 ? cells-1 : col + 1);
		break;
	case 75:
		//left
		col = (col==0 ? 0 : col - 1);
		break;
	case 80:
		//down
		row = (row==cells-1 ? cells - 1 : row + 1);
		break;
	case 13:
		//enter
		if (board[row][col] == -1) {
			board[row][col] = turn;
			boardCount++;
			toggleTurn();
			getWinner();
			if (win != -1 || boardCount == cells * cells) {
				*game = 0;
				draw(1);
			}
		}
		break;
	}
}

//	Toggle turn
void Tic::toggleTurn() {
	turn = !turn;
}

//	End Game
void Tic::endGame() {
	std::cout << Color(4) << "GAME OVER!" << std::endl;
	if (win == -1) {
		std::cout << Color(2) << "DRAW!!!" << std::endl;
	}
	else {
		std::cout << (win+1==1 ? Color(9) : Color(6)) << "PLAYER " << win+1 << " Wins!!!" << std::endl;
	}
	std::cout << Color(7) << "Press any key to exit . . ." << std::endl;
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