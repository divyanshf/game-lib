#include "Menu.h"

Menu::Menu(SDL_Renderer* ren, SDL_Window* win) {
	//	Initialize SDL
	this->ren = ren;
	this->win = win;
	SDL_GetWindowSize(this->win, &winWidth, &winHeight);

	//	Initialize Audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Failed at Mi_OpenAudio()" << std::endl;
	}
	clickEffect = Mix_LoadWAV("Audio/Click.wav");
	startEffect = Mix_LoadWAV("Audio/Start.wav");

	//	Initialize TTF
	TTF_Init();
	head = "GameLib";
	question = "What do you want to play ?";
	headFont = TTF_OpenFont("Fonts/Chopsic/Chopsic-K6Dp.ttf", 48);
	questionFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 24);
	optionFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 18);

	//	Initialize options
	list.push_back(tupleStr("TIC-TAC-TOE", "Assets/GameImage/Tic.png"));
	list.push_back(tupleStr("PONG", "Assets/GameImage/Pong.png"));
	list.push_back(tupleStr("HANGMAN", "Assets/GameImage/HangMan.png"));
	list.push_back(tupleStr("EXIT", "Assets/GameImage/Exit.png"));
	listOption = list.begin();

	//	Initialize gameImage
	gameImage = new Object();
	gameImage->setDest(1 + 10, 100 + 10, 180, 180);
	gameImage->setImage(std::get<1>(*listOption), ren);
	gameImage->setSource(0, 0, 200, 200);
	running = 1;
}
Menu::~Menu() {
	Mix_FreeChunk(clickEffect);
	Mix_FreeChunk(startEffect);
	Mix_Quit();
	TTF_CloseFont(headFont);
	TTF_CloseFont(questionFont);
	TTF_CloseFont(optionFont);
	TTF_Quit();
}
int Menu::loop() {
	vecTupleStr::iterator choice;
	while (running) {
		render();
		choice = input();
		if (choice != list.end()) {
			return std::distance(list.begin(), listOption);
		}
	}
}

void Menu::render() {
	SDL_RenderClear(ren);

	int textWidth, textHeight;

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_Rect rect;
	rect.w = winWidth;
	rect.h = winHeight;
	rect.x = 0;
	rect.y = 0;
	SDL_RenderFillRect(ren, &rect);

	//	Head Text
	TTF_SizeText(headFont, head, &textWidth, &textHeight);
	draw("GameLib", headFont, (winWidth / 2) - (textWidth / 2), 10, 255, 255, 0);

	//	Question text
	TTF_SizeText(questionFont, question, &textWidth, &textHeight);
	draw("What do you want to play ?", questionFont, (winWidth / 2) - (textWidth / 2), 100, 0, 0, 255);

	//	Options Text
	int j = 0;
	const char* arrow = "->";
	const char* space = "  ";
	for (vecTupleStr::iterator i = list.begin(); i != list.end(); i++) {
		char text[256];
		int r = 255, g = 0, b = 0;
		if (listOption == i) {
			strcpy_s(text, arrow);
			r = 0;
			g = 153;
		}
		else {
			strcpy_s(text, space);
		}
		const char* tmp = std::get<0>(*i);
		strcat_s(text, tmp);
		TTF_SizeText(optionFont, text, NULL, &textHeight);
		draw(text, optionFont, (winWidth / 2) - 50, 150 + j * (textHeight + 15), r, g, b);
		j++;
	}

	//	TIC-TAC-TOE Lines
	SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(ren, 1, 100, 201, 100);
	SDL_RenderDrawLine(ren, 1, 300, 201, 300);
	SDL_RenderDrawLine(ren, 1, 100, 1, 300);
	SDL_RenderDrawLine(ren, 201, 100, 201, 300);


	//	Image
	SDL_Rect dest = gameImage->getDest();
	SDL_Rect src = gameImage->getSource();
	SDL_RenderCopyEx(ren, gameImage->getTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);

	SDL_RenderPresent(ren);
}

void Menu::draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b) {
	SDL_Surface* surfMsg;
	SDL_Texture* texMsg;
	SDL_Rect rect;
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = 255;
	surfMsg = TTF_RenderText_Solid(font, msg, color);
	texMsg = SDL_CreateTextureFromSurface(ren, surfMsg);
	rect.x = x;
	rect.y = y;
	rect.w = surfMsg->w;
	rect.h = surfMsg->h;
	SDL_RenderCopy(ren, texMsg, NULL, &rect);
	SDL_FreeSurface(surfMsg);
	SDL_DestroyTexture(texMsg);
}

vecTupleStr::iterator Menu::input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = 0;
			listOption = list.end() - 1;
			return listOption;
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_UP) {
				listOption = listOption == list.begin() ? list.end() - 1 : listOption - 1;
				gameImage->setImage(std::get<1>(*listOption), ren);
				Mix_PlayChannel(-1, clickEffect, 0);
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				listOption = listOption == list.end() - 1 ? list.begin() : listOption + 1;
				gameImage->setImage(std::get<1>(*listOption), ren);
				Mix_PlayChannel(-1, clickEffect, 0);
			}
			if (event.key.keysym.sym == SDLK_RETURN) {
				Mix_PlayChannel(-1, startEffect, 0);
				return listOption;
			}
		}
	}
	return list.end();
}