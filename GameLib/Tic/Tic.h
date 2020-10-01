#ifndef TIC_H
#define TIC_H

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "TicBoard.h"

typedef std::vector <const char*> vecString;

class Tic
{
private:
	SDL_Renderer* ren;
	SDL_Window* win;
	TTF_Font* titleFont, * normalFont, * playerFont, * instructionFont;
	Mix_Music* bgm;
	Mix_Chunk* clickEffect, * loseEffect, * startEffect, * winnerEffect, * collideEffect;
	TicBoard b;
	Cord boardCords[3][3];
	vecString ticOptions;
	vecString::iterator ticOption;
	int winWidth, winHeight;
	int game;
	const char* head;
	int nPlayers;
	int running;
public:
	Tic(SDL_Renderer* ren, SDL_Window* win);
	~Tic();
	void ticMenu();
	void inputMenu();
	void renderMenu();
	void loop();
	void render();
	void draw();
	void draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b);
	void input();
};

#endif // !TIC_H