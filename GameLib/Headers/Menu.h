#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include "./Color.h"

typedef std::vector<std::string>::size_type vectorSize;

class Menu
{
private:
	std::vector <std::string> options = {"TIC-TAC-TOE", "CLASSSSIC SNAKE", "EXIT"};
	vectorSize option;
	vectorSize optionSize;
public:
	Menu();
	void drawMenu();
	vectorSize input();
};

Menu::Menu(){
	option = 0;
	optionSize = options.size();
}

void Menu::drawMenu() {
	system("cls");
	std::cout << Color(14) << "< < < WELCOME TO GAMELIB > > >\n" << Color(7) << std::endl;
	std::cout << Color(3) << "Choose one of the following games:" << Color(7) << std::endl;
	for (vectorSize i = 0; i != optionSize; i++) {
		std::cout << (option==i ? Color(2) : Color(7)) << (option==i ? "-> " : "   ") << options[i] << Color(7) << std::endl;
	}
}

vectorSize Menu::input() {
	int ch = _getch();
	if (ch == 72) {
		option = (option == 0 ? 0 : option-1);
	}
	else if (ch == 80) {
		option = (option == optionSize-1 ? optionSize-1 : option+1);
	}
	else if (ch == 13) {
		return option;
	}
	return -1;
}
