#define SDL_MAIN_HANDLED

#include <SDL.h>
#include "Menu/Menu.h"
#include "Tic/Tic.h"
#include "Pong/Pong.h"

void runTic(SDL_Renderer* ren, SDL_Window* win) {
	Tic obj(ren, win);
	obj.ticMenu();
	obj.loop();
}

void runPong(SDL_Renderer* ren, SDL_Window* win) {
	Pong obj(ren, win);
	obj.loop();
}

int main() {
	SDL_Renderer* ren;
	SDL_Window* win;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed at SDL_Init" << std::endl;
	}
	if (SDL_CreateWindowAndRenderer(1024, 720, SDL_WINDOW_FULLSCREEN, &win, &ren) < 0) {
		std::cout << "Failed at SDL_CreateWindowAndRenderer" << std::endl;
	}
	SDL_SetWindowTitle(win, "GameLib");
	while (1) {
		Menu m(ren, win);
		int choice = m.loop();
		switch (choice) {
		case 0:
			runTic(ren, win);
			break;
		case 1:
			runPong(ren, win);
			break;
		case 2:
			exit(0);
			break;
		}
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}