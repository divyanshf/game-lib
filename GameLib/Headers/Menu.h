#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <windows.h>
#include <Lmcons.h>
#include "./Color.h"

typedef std::vector<std::string>::size_type vectorSize;

class Menu
{
private:
	std::vector <std::string> options = {"TIC-TAC-TOE", "PONG", "EXIT"};
	vectorSize option;
	vectorSize optionSize;
	char username[UNLEN + 1];
	DWORD username_len;
public:
	Menu();
	void drawMenu();
	vectorSize input();
};

Menu::Menu(){
	option = 0;
	optionSize = options.size();
	username_len = UNLEN + 1;
	GetUserName((LPSTR)username, &username_len);
}

void Menu::drawMenu() {
	system("cls");
	std::cout << Color(14) << "< < < WELCOME TO GAMELIB > > >\n" << Color(7) << std::endl;
	std::cout << "User: " << Color(4) << username << Color(7) << std::endl;
	std::cout << Color(3) << "\nChoose one of the following games:" << Color(7) << std::endl;
	for (vectorSize i = 0; i != optionSize; i++) {
		std::cout << (option==i ? Color(2) : Color(7)) << (option==i ? "-> " : "   ") << options[i] << Color(7) << std::endl;
	}
}

vectorSize Menu::input() {
	int ch = _getch();
	if (ch == 72) {
		option = (option == 0 ? optionSize-1 : option-1);
	}
	else if (ch == 80) {
		option = (option == optionSize-1 ? 0 : option+1);
	}
	else if (ch == 13) {
		return option;
	}
	return -1;
}
