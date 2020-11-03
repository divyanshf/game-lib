#ifndef FLAPPY_H
#define FLAPPY_H

#include <io.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../Object/Object.h"

struct pipe {
	SDL_Rect top;
	SDL_Rect bottom;
	int x;
};

typedef std::vector <pipe> vecPipe;

class Flappy
{
private:
	SDL_Renderer* ren;
	SDL_Window* win;
	TTF_Font* titleFont, * normalFont, * playerFont, * instructionFont;
	Mix_Music* bgm;
	Mix_Chunk* clickEffect, * loseEffect, * winnerEffect, * errorEffect;
	Object volume, bird;
	bool isMute;
	int winWidth, winHeight;
	int FPS;
	int frameDelay, frameTime;
	Uint32 frameStart;
	int game;
	int score, bestScore;
	int velocity, gravity, lift, isLift, liftCounter;
	int birdHeight;
	int pipeWidth, pipeGap, pipeSpeed;
	vecPipe pipes;
	const char* head;
	int running;
	std::string username;
	std::string scoreFileName;
	std::fstream scoreFile;
	void render();
	void update();
	void updateAnimation();
	void addPipes();
	void draw();
	void draw(Object obj);
	void draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b);
	void input();
public:
	Flappy(SDL_Renderer* ren, SDL_Window* win, std::string username);
	~Flappy();
	void loop();
};


#endif // !FLAPPY_H