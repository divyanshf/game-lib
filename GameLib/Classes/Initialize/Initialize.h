#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <iostream>
#include <SDL.h>

class Initialize
{
private:
	SDL_Renderer* ren;
	SDL_Window* win;
public:
	Initialize();
	~Initialize();
	SDL_Renderer* getRenderer() {
		return ren;
	}
	SDL_Window* getWindow() {
		return win;
	}
};


#endif // !INITIALIZE_H