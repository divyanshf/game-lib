#define SDL_MAIN_HANDLED

#include <SDL.h>
#include "Classes/Initialize/Initialize.h"
#include "Classes/Menu/Menu.h"
#include "Classes/Tic/Tic.h"
#include "Classes/Pong/Pong.h"
#include "Classes/Hangman/Hang.h"
#include "Classes/Snake/Snake.h"
#include "Classes/Flappy/Flappy.h"
#include "Classes/User/User.h"

//	Tic-Tac-Toe
void runTic(SDL_Renderer* ren, SDL_Window* win, std::string username) {
	Tic obj(ren, win, username);
	obj.ticMenu();
	obj.loop();
}

//	Pong
void runPong(SDL_Renderer* ren, SDL_Window* win, std::string username) {
	Pong obj(ren, win, username);
	obj.loop();
}

//	Hangman
void runHang(SDL_Renderer* ren, SDL_Window* win, std::string username) {
	Hang obj(ren, win, username);
	obj.loop();
}

//	Snake
void runSnake(SDL_Renderer* ren, SDL_Window* win, std::string username) {
	Snake obj(ren, win, username);
	obj.loop();
}

//	Flappy
void runFlappy(SDL_Renderer* ren, SDL_Window* win, std::string username) {
	Flappy obj(ren, win, username);
	obj.loop();
}

int main() {
	int window = 1;
	std::string username = "";

	//	Initialize SDL stuff
	Initialize init;

	SDL_Renderer* ren = init.getRenderer();
	SDL_Window* win = init.getWindow();

	std::cout << "Application is starting . . ." << std::endl;

	//	User Menu
	User user(ren, win);
	username = user.loop();
	//username = "test";

	//	Check username
	if (username != "") {
		while (window) {
			//	Game menu
			Menu m(ren, win, username);
			int choice = m.loop();

			//	Particular game
			switch (choice) {
			case 0:
				runTic(ren, win, username);
				break;
			case 1:
				runPong(ren, win, username);
				break;
			case 2:
				runHang(ren, win, username);
				break;
			case 3:
				runSnake(ren, win, username);
				break;
			case 4:
				runFlappy(ren, win, username);
				break;
			case 5:
				window = 0;
				break;
			}
		}
	}

	return 0;
}