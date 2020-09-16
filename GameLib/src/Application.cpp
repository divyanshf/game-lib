#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "Color.h"
#include "Menu.h"
#include "Tic.h"
#include "Pong.h"

//	tic-tac-toe
void ticTacToe() {
	int players, turn;
	int ch;
	char player1, player2;
	TicMenu menuObj;
	std::tuple <int, int, int> ticInfo = menuObj.run();
	players = std::get<0>(ticInfo);
	ch = std::get<1>(ticInfo);
	if (ch == 0) {
		player1 = 'O';
		player2 = 'X';
	}
	else {
		player1 = 'X';
		player2 = 'O';
	}
	turn = std::get<2>(ticInfo);
	std::cout << player1 << " " << player2 << std::endl;
	Tic obj(player1, player2, turn, players);
	obj.run();
}

void pongMain() {
	GameManager game(40, 20);
	game.run();
}

int main() {
	std::vector <std::string>::size_type option;
	Menu menuObj;
	while(1) {
		menuObj.drawMenu();
		option = menuObj.input();
		switch (option) {
		case 0:
			ticTacToe();
			break;
		case 1:
			pongMain();
			break;
		case 2:
			system("cls");
			std::cout << Color(4) << "Stopping the process . . .\n" << Color(7);
			Sleep(2000);
			exit(0);
			break;
		default:
			break;
		}
	}
}