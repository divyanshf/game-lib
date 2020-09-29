#include "Tic.h"

Tic::Tic() : b("O", "X") {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed at SDL_Init" << std::endl;
	}
	if (SDL_CreateWindowAndRenderer(800, 640, SDL_WINDOW_FULLSCREEN, &win, &ren) < 0) {
		std::cout << "Failed at SDL_CreateWindowAndRenderer" << std::endl;
	}
	SDL_SetWindowTitle(win, "GameLib : Tic-Tac-Toe");
	SDL_GetWindowSize(win, &winWidth, &winHeight);
	TTF_Init();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCords[i][j] = Cord(winWidth / 2 - ((1 - j) * 100), winHeight / 2 - ((1 - i) * 100));
		}
	}
	ticOptions.push_back("You  v/s  Computer");
	ticOptions.push_back("Player 1  v/s  Player 2");
	ticOption = ticOptions.begin();
	head = "Tic-Tac-Toe";
	titleFont = TTF_OpenFont("Fonts/Chopsic/Chopsic-K6Dp.ttf", 48);
	normalFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 24);
	nPlayers = 1;
	game = 1;
	running = 1;
}

Tic::~Tic() {
	TTF_CloseFont(titleFont);
	TTF_CloseFont(normalFont);
	TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void Tic::ticMenu() {
	while (game) {
		renderMenu();
		inputMenu();
	}
}

void Tic::inputMenu() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = 0;
			std::cout << "Quitting" << std::endl;
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				running = 0;
				std::cout << "Quitting" << std::endl;
			}
			if (event.key.keysym.sym == SDLK_UP) {
				ticOption = (ticOption == ticOptions.begin() ? ticOptions.end() : ticOption - 1);
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				ticOption = (ticOption == ticOptions.end() - 1 ? ticOptions.begin() : ticOption + 1);
			}
			if (event.key.keysym.sym == SDLK_RETURN) {
				nPlayers = (ticOption == ticOptions.begin() ? 1 : 2);
				game = 0;
			}
		}
	}
}

void Tic::renderMenu() {
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	int textWidth, textHeight;

	SDL_Rect rect;
	rect.w = winWidth;
	rect.h = winHeight;
	rect.x = 0;
	rect.y = 0;
	SDL_RenderFillRect(ren, &rect);

	//	Head Text
	TTF_SizeText(titleFont, head, &textWidth, NULL);
	draw(head, titleFont, (winWidth / 2) - (textWidth / 2), 10, 255, 255, 0);

	//	Question text
	TTF_SizeText(normalFont, "How many players ?", &textWidth, NULL);
	draw("How many players ?", normalFont, (winWidth / 2) - (textWidth / 2), 100, 0, 0, 255);

	//	Options
	int j = 1;
	for (vecString::iterator i = ticOptions.begin(); i != ticOptions.end(); i++) {
		TTF_SizeText(normalFont, *(i), &textWidth, NULL);
		int r = 255, g = 0, b = 0;
		if (i == ticOption) {
			r = 0;
			g = 255;
			draw("->", normalFont, (winWidth / 2) - (textWidth / 2) - 20, 200 + j * (50), r, g, b);
		}
		draw(*(i), normalFont, (winWidth / 2) - (textWidth / 2), 200 + j * (50), r, g, b);
		j++;
	}

	SDL_RenderPresent(ren);
}

void Tic::loop() {
	if (nPlayers == 2) {
		while (running) {
			render();
			input();
		}
	}
	else {
		while (running) {
			render();
			if (b.getTurn() == 0) {
				game = b.aiMove();
			}
			else {
				input();
			}
		}
	}
}

void Tic::render() {
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	int textWidth, textHeight;

	SDL_Rect rect;
	rect.w = winWidth;
	rect.h = winHeight;
	rect.x = 0;
	rect.y = 0;
	SDL_RenderFillRect(ren, &rect);

	//	Head Text
	TTF_SizeText(titleFont, head, &textWidth, NULL);
	draw(head, titleFont, (winWidth / 2) - (textWidth / 2), 10, 255, 255, 0);

	draw();

	SDL_RenderPresent(ren);

	if (game) {
		SDL_Delay(3000);
		running = 0;
	}
}

void Tic::draw() {
	int textWidth, textHeight;

	SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(ren, (winWidth / 2 - 50), (winHeight / 2) - 150, (winWidth / 2 - 50), (winHeight / 2 + 150));
	SDL_RenderDrawLine(ren, (winWidth / 2 + 50), (winHeight / 2) - 150, (winWidth / 2 + 50), (winHeight / 2 + 150));
	SDL_RenderDrawLine(ren, (winWidth / 2 - 150), (winHeight / 2) - 50, (winWidth / 2 + 150), (winHeight / 2 - 50));
	SDL_RenderDrawLine(ren, (winWidth / 2 - 150), (winHeight / 2) + 50, (winWidth / 2 + 150), (winHeight / 2 + 50));
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int x = std::get<0>(boardCords[i][j]);
			int y = std::get<1>(boardCords[i][j]);
			const char* ch = b.getBoardChar(i, j);
			TTF_SizeText(normalFont, ch, &textWidth, &textHeight);
			int r = 255, g = 255, b = 255;
			if (ch == "O") {
				g = 153;
				b = 51;
			}
			else if (ch == "X") {
				r = 51;
				g = 153;
			}
			draw(ch, normalFont, x - textWidth / 2, y - textHeight / 2, r, g, b);
		}
	}

	if (game) {
		const char* player = b.getResult();
		const char* sent = (player == "D" ? "Its a draw !!!" : " is the winner !!!");
		if (player == "D") {
			TTF_SizeText(normalFont, sent, &textWidth, &textHeight);
			draw(sent, normalFont, winWidth / 2 - textWidth / 2, winHeight / 2 + 200, 0, 0, 255);
		}
		else {
			char result[100];
			strcpy_s(result, player);
			strcat_s(result, sent);
			TTF_SizeText(normalFont, result, &textWidth, &textHeight);
			draw(result, normalFont, winWidth / 2 - textWidth / 2, winHeight / 2 + 200, 0, 255, 0);
		}
	}
}

void Tic::draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b) {
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

void Tic::input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = 0;
			std::cout << "Quitting" << std::endl;
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				running = 0;
				std::cout << "Quitting" << std::endl;
			}
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (!game) {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						int x = std::get<0>(boardCords[i][j]);
						int y = std::get<1>(boardCords[i][j]);
						if (event.motion.x > x - 50 && event.motion.x < x + 50) {
							if (event.motion.y > y - 50 && event.motion.y < y + 50) {
								game = b.humanMove(i, j);
							}
						}
					}
				}
			}
		}
	}
}