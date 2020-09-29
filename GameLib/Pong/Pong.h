#ifndef PONG_H
#define PONG_H

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Ball.h"
#include "Paddle.h"

class Pong
{
private:
	SDL_Renderer* ren;
	SDL_Window* win;
	TTF_Font* titleFont, * normalFont;
	int winWidth, winHeight;
	const char* head;
	int running;
	int gameWidth, gameHeight;
	Ball* ball;
	Paddle* player1;
	Paddle* player2;
	int score1, score2;
public:
	Pong();
	~Pong();
	void loop();
	void render();
	void draw(Object obj);
	void draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b);
	void logic();
	void input();
};


#endif // !PONG_H