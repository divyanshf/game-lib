#ifndef HANG_H
#define HANG_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <conio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "../Object/Object.h"

class Hang
{
private:
	SDL_Renderer* ren;
	SDL_Window* win;
	TTF_Font* titleFont, * normalFont, * playerFont, * instructionFont;
	Mix_Music* bgm;
	Mix_Chunk* clickEffect, * loseEffect, * winnerEffect, *thankEffect, *screamEffect, *errorEffect;
	Object volume;
	bool isMute; 
	int winWidth, winHeight;
	int FPS;
	int frameDelay, frameTime;
	Uint32 frameStart;
	int game;
	int winner;
	Object anim;
	std::fstream file;
	std::string word;
	std::vector <std::string> words;
	std::string guess;
	int chances;
	const char* head;
	int running;
	std::string username;
	void render();
	void draw();
	void draw(Object obj);
	void draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b);
	int getWinner();
	void updateAnimation();
	void input();
public:
	Hang(SDL_Renderer* ren, SDL_Window* win, std::string username);
	~Hang();
	void loop();
};

#endif // !HANG_H