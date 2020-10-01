#ifndef PONG_H
#define PONG_H

#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Ball.h"
#include "Paddle.h"

typedef std::vector <const char*> vecString;

class Pong
{
private:
	SDL_Renderer* ren;
	SDL_Window* win;
	TTF_Font* titleFont, * normalFont, * instructionFont;
	Mix_Music* bgm;
	Mix_Chunk* collideEffect, * loseEffect, * startEffect, * winnerEffect, * wallEffect;
	int winWidth, winHeight;
	const char* head;
	int running, game;
	int gameWidth, gameHeight;
	Ball* ball;
	Paddle* player1;
	Paddle* player2;
	int score1, score2;
	vecString pongOptions;
	vecString::iterator pongOption;
	void render();
	void draw(int titleHeight);
	void draw(Object obj);
	void draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b);
	void logic();
	void input();
	void scoreUp(Paddle* player);
public:
	Pong(SDL_Renderer* ren, SDL_Window* win);
	~Pong();
	void loop();
};


#endif // !PONG_H