#define SDL_MAIN_HANDLED

#include <SDL.h>
#include "Menu/Menu.h"
#include "Tic/Tic.h"
#include "Pong/Pong.h"
#include "Hangman/Hang.h"
#include "Snake/Snake.h"
#include "User/User.h"

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

int main() {
	int window = 1;
	std::string username = "";

	//	Inititialize the sdl renderer
	SDL_Renderer* ren;
	//	Inititialize the sdl window
	SDL_Window* win;

	//	Initialize sdl
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed at SDL_Init" << std::endl;
	}
	//	Create window and renderer
	if (SDL_CreateWindowAndRenderer(1024, 720, SDL_WINDOW_FULLSCREEN, &win, &ren) < 0) {
		std::cout << "Failed at SDL_CreateWindowAndRenderer" << std::endl;
	}
	SDL_SetWindowTitle(win, "GameLib");

	//	User Menu
	User user(ren, win);
	username = user.loop();
	
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
				window = 0;
				break;
			}
		}
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}