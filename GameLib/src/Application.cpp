#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "Color.h"
#include "Menu.h"
#include "Tic.h"

//	tic-tac-toe
void ticTacToe() {
	Tic obj('O', 'X', 0);
	obj.run();
}

void pongMain();

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
			std::cin.get();
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