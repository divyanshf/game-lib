#include <iostream>
#include "../Headers/Tic.h"

void ticMain() {
	Tic ticObj;
	int game = 1;
	while (game) {
		ticObj.draw(0);
		ticObj.playChance(&game);
	}
	std::cin.get();
}