#include <iostream>

void ticMain();

int main() {
	int option;
	do {
		std::cout << "< < <\tWELCOME\t> > >" << std::endl;
		std::cout << "Choose one of the following games:" << std::endl;
		std::cout << "1. TIC-TAC-TOE" << std::endl;
		std::cin >> option;
		switch (option) {
		case 1:
			system("cls");
			ticMain();
			std::cin.get();
			break;
		default:
			break;
		}
		system("cls");
	} while (1);
}