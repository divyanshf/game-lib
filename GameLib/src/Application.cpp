#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "../Headers/Color.h"
#include "../Headers/Menu.h"

void ticMain();
void snakeMain();

int main() {
	int option;
	Menu menuObj;
	while(1) {
		menuObj.drawMenu();
		option = menuObj.input();
		switch (option) {
		case 0:
			ticMain();
			std::cin.get();
			break;
		case 1:
			snakeMain();
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