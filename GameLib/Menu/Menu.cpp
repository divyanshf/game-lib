#include "Menu.h"

Menu::Menu() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed at SDL_Init" << std::endl;
	}
	if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &win, &ren) < 0) {
		std::cout << "Failed at SDL_CreateWindowAndRenderer" << std::endl;
	}
	SDL_SetWindowTitle(win, "GameLib");

	TTF_Init();
	head = "GameLib";
	question = "What do you want to play ?";
	headFont = TTF_OpenFont("Fonts/Chopsic/Chopsic-K6Dp.ttf", 48);
	questionFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 24);
	optionFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 18);
	list.push_back("TIC-TAC-TOE");
	list.push_back("PONG");
	list.push_back("EXIT");
	listOption = list.begin();
	running = 1;
}
Menu::~Menu() {
	TTF_CloseFont(headFont);
	TTF_CloseFont(questionFont);
	TTF_CloseFont(optionFont);
	TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
int Menu::loop() {
	vecString::iterator choice;
	while (running) {
		render();
		choice = input();
		if (choice != list.end()) {
			return std::distance(list.begin(), listOption);
		}
	}
}
void Menu::render() {
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	int textWidth, textHeight;

	SDL_Rect rect;
	rect.w = WIDTH;
	rect.h = HEIGHT;
	rect.x = 0;
	rect.y = 0;
	SDL_RenderFillRect(ren, &rect);

	//	Head Text
	TTF_SizeText(headFont, head, &textWidth, NULL);
	draw("GameLib", headFont, (WIDTH / 2) - (textWidth / 2), 10, 255, 255, 0);

	//	Question text
	TTF_SizeText(questionFont, question, &textWidth, NULL);
	draw("What do you want to play ?", questionFont, (WIDTH / 2) - (textWidth / 2), 100, 0, 0, 255);

	//	Options Text
	int j = 0;
	const char* arrow = "->";
	const char* space = "  ";
	for (vecString::iterator i = list.begin(); i != list.end(); i++) {
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
		strcat_s(text, *i);
		TTF_SizeText(optionFont, text, NULL, &textHeight);
		draw(text, optionFont, (WIDTH / 2) - 50, 150 + j * (textHeight + 15), r, g, b);
		j++;
	}

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
vecString::iterator Menu::input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = 0;
			std::cout << "Please Select the Exit Option : " << *(list.end() - 1) << std::endl;
			listOption = list.end() - 1;
			return listOption;
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_UP) {
				listOption = listOption == list.begin() ? list.end() - 1 : listOption - 1;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				listOption = listOption == list.end() - 1 ? list.begin() : listOption + 1;
			}
			if (event.key.keysym.sym == SDLK_RETURN) {
				return listOption;
			}
		}
	}
	return list.end();
}