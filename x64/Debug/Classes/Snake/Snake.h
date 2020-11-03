#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include "../Object/Object.h"

typedef std::vector<Object> vecObject;
typedef std::vector<int> vecObjectPos;

enum class sDir {
	STOP = 0,
	LEFT = 1,
	RIGHT = 2,
	UP = 3,
	DOWN = 4
};

class Snake
{
private:
	SDL_Renderer* ren;
	SDL_Window* win;
	int winWidth, winHeight;
	Mix_Music* bgm;
	Mix_Chunk* loseEffect, * winnerEffect, * gulpEffect;
	Object volume;
	int isMute;
	TTF_Font* titleFont, * normalFont, * instructionFont;
	const char* head;
	int FPS;
	int frameDelay, frameTime;
	Uint32 frameStart;
	Object fruit;
	Object sShape;
	vecObject snake;
	vecObjectPos x, y;
	sDir direction;
	int nTail;
	int grids;
	std::string username;
	int mainX, mainY;	//	Grid (0, 0)
	int posX, posY;	//	Snake Position
	int fruitX, fruitY;	//	Fruit Position
	std::string scoreFileName;
	std::ofstream oScoreFile;
	std::ifstream iScoreFile;
	int score, bestScore;
	int running, game;
public:
	Snake(SDL_Renderer* ren, SDL_Window* win, std::string username);
	~Snake();
	void loop();
	void render();
	void logic();
	void draw(Object obj);
	void draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b);
	void input();
};


#endif // !SNAKE_H