#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <tuple>
#include "../Object/Object.h"

typedef std::tuple <const char*, const char*> tupleStr;
typedef std::vector <tupleStr> vecTupleStr;

class Menu
{
private:
	SDL_Renderer* ren;
	SDL_Window* win;
	int winWidth, winHeight;
	Mix_Chunk* clickEffect, * startEffect;
	TTF_Font* headFont, * questionFont, * optionFont;
	const char* head, * question;
	vecTupleStr list;
	vecTupleStr::iterator listOption;
	Object* gameImage;
	int running;
public:
	Menu(SDL_Renderer* ren, SDL_Window* win);
	~Menu();
	int loop();
	void render();
	void draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b);
	vecTupleStr::iterator input();
};


#endif // !MENU_H