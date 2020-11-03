#ifndef USER_H
#define USER_H

#define errno   (*_errno())

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <direct.h>

class User
{
private:
	SDL_Renderer* ren;
	SDL_Window* win;
	int winWidth, winHeight;
	Mix_Chunk* clickEffect, * startEffect;
	TTF_Font* headFont, * normalFont, * userFont, * instructionFont;
	const char* head;
	std::string username;
	std::string password;
	std::vector<char> forbid;
	std::fstream passFile;
	bool error, putPassword, showPass;
	int running, quit;
	void encrypt();
	std::string decrypt();
public:
	User(SDL_Renderer* ren, SDL_Window* win);
	~User();
	int checkError();
	void render();
	void draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b);
	std::string loop();
	void input();
};


#endif // !USER_H