#include "./Pong.h"

void pongMain() {
	GameManager game(40, 20);
	game.run();
	std::cin.get();
}