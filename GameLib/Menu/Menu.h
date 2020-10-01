#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>

typedef std::vector <const char*> vecString;

class Menu
{
private:
	SDL_Renderer* ren;
	SDL_Window* win;
	int winWidth, winHeight;
	Mix_Chunk* clickEffect, * startEffect;
	TTF_Font* headFont, * questionFont, * optionFont;
	const char* head, * question;
	vecString list;
	vecString::iterator listOption;
	int running;
public:
	Menu(SDL_Renderer* ren, SDL_Window* win);
	~Menu();
	int loop();
	void render();
	void draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b);
	vecString::iterator input();
};


#endif // !MENU_H