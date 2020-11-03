#include "Initialize.h"

Initialize::Initialize() {
	
	//	Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed at SDL_Init" << std::endl;
	}
	//	Create window and renderer
	if (SDL_CreateWindowAndRenderer(1024, 720, SDL_WINDOW_FULLSCREEN, &win, &ren) < 0) {
		std::cout << "Failed at SDL_CreateWindowAndRenderer" << std::endl;
	}
	SDL_SetWindowTitle(win, "GameLib");
}

Initialize::~Initialize() {
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}