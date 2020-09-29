#include "Tic.h"

//	Tic Contstuctor
Tic::Tic() : b("O", "X") {
	//	Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed at SDL_Init" << std::endl;
	}

	//	Create window and renderer
	if (SDL_CreateWindowAndRenderer(800, 640, SDL_WINDOW_FULLSCREEN, &win, &ren) < 0) {
		std::cout << "Failed at SDL_CreateWindowAndRenderer" << std::endl;
	}

	//	Set window title
	SDL_SetWindowTitle(win, "GameLib : Tic-Tac-Toe");

	//	Set window size
	SDL_GetWindowSize(win, &winWidth, &winHeight);
	
	//	Initialize TTF
	TTF_Init();

	//	Initialize boardCords
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCords[i][j] = Cord(winWidth / 2 - ((1 - j) * 100), winHeight / 2 - ((1 - i) * 100));
		}
	}

	//	Initialize ticOptions
	ticOptions.push_back("You  v/s  Computer");
	ticOptions.push_back("Player 1  v/s  Player 2");
	ticOption = ticOptions.begin();

	//	Initialize title and fonts
	head = "Tic-Tac-Toe";
	titleFont = TTF_OpenFont("Fonts/Chopsic/Chopsic-K6Dp.ttf", 48);
	normalFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 24);
	playerFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 56);
	
	//	Initialize remaining variables
	nPlayers = 1;
	game = 1;
	running = 1;
}

//	Tic destructor
Tic::~Tic() {
	TTF_CloseFont(titleFont);
	TTF_CloseFont(normalFont);
	TTF_CloseFont(playerFont);
	TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

//	Tic Menu 
void Tic::ticMenu() {
	while (game) {
		renderMenu();
		inputMenu();
	}

	//	Clear the renderer
	SDL_RenderClear(ren);
}

//	Tic Menu Input
void Tic::inputMenu() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		//	Quit button
		if (event.type == SDL_QUIT) {
			running = 0;
			std::cout << "Quitting" << std::endl;
		}

		//	On key press
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_UP) {
				ticOption = (ticOption == ticOptions.begin() ? ticOptions.end() - 1 : ticOption - 1);
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				ticOption = (ticOption == ticOptions.end() - 1 ? ticOptions.begin() : ticOption + 1);
			}
			if (event.key.keysym.sym == SDLK_RETURN) {
				nPlayers = std::distance(ticOptions.begin(), ticOption) + 1;
				game = 0;
			}
		}
	}
}

//	Tic Menu render
void Tic::renderMenu() {
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	int textWidth, textHeight;

	//	Background rect
	SDL_Rect rect;
	rect.w = winWidth;
	rect.h = winHeight;
	rect.x = 0;
	rect.y = 0;
	SDL_RenderFillRect(ren, &rect);

	//	Head Text
	TTF_SizeText(titleFont, head, &textWidth, NULL);
	draw(head, titleFont, (winWidth / 2) - (textWidth / 2), 10, 255, 255, 0);

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

	//	Render the items
	SDL_RenderPresent(ren);
}

//	Tic Loop
void Tic::loop() {

	//	Two players
	if (nPlayers == 2) {
		while (running) {
			render();
			input();
		}
	}

	//	Single player
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

//	Tic render
void Tic::render() {
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	int textWidth, textHeight;

	//	Background rectangle
	SDL_Rect rect;
	rect.w = winWidth;
	rect.h = winHeight;
	rect.x = 0;
	rect.y = 0;
	SDL_RenderFillRect(ren, &rect);

	//	Head Text
	TTF_SizeText(titleFont, head, &textWidth, NULL);
	draw(head, titleFont, (winWidth / 2) - (textWidth / 2), 10, 255, 255, 0);

	//	Board and players
	draw();

	//	Render the items
	SDL_RenderPresent(ren);

	//	End game
	if (game) {
		SDL_Delay(3000);
		running = 0;
	}
}

//	Tic draw 
void Tic::draw() {
	int textWidth, textHeight;

	SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);

	//	TIC-TAC-TOE Lines
	SDL_RenderDrawLine(ren, (winWidth / 2 - 50), (winHeight / 2) - 150, (winWidth / 2 - 50), (winHeight / 2 + 150));
	SDL_RenderDrawLine(ren, (winWidth / 2 + 50), (winHeight / 2) - 150, (winWidth / 2 + 50), (winHeight / 2 + 150));
	SDL_RenderDrawLine(ren, (winWidth / 2 - 150), (winHeight / 2) - 50, (winWidth / 2 + 150), (winHeight / 2 - 50));
	SDL_RenderDrawLine(ren, (winWidth / 2 - 150), (winHeight / 2) + 50, (winWidth / 2 + 150), (winHeight / 2 + 50));
	
	//	Board items
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

	//	Player 1 Text
	TTF_SizeText(titleFont, b.getPlayer1(), &textWidth, &textHeight);
	draw(b.getPlayer1(), playerFont, (winWidth / 2 - 150) / 2 - textWidth / 2, winHeight / 2 - textHeight / 2, 255, 153, 51);
	
	if (nPlayers == 2) {
		TTF_SizeText(normalFont, "Player 1", &textWidth, &textHeight);
		draw("Player 1", normalFont, (winWidth / 2 - 150) / 2 - textWidth / 2, winHeight / 2 - textHeight / 2 + 100, 255, 153, 51);
	}
	else {
		TTF_SizeText(normalFont, "Computer", &textWidth, &textHeight);
		draw("Computer", normalFont, (winWidth / 2 - 150) / 2 - textWidth / 2, winHeight / 2 - textHeight / 2 + 100, 255, 153, 51);
	}


	//	Player 2 Text
	TTF_SizeText(titleFont, b.getPlayer2(), &textWidth, &textHeight);
	draw(b.getPlayer2(), playerFont, ((winWidth / 2 + 150) + winWidth) / 2 - textWidth / 2, winHeight / 2 - textHeight / 2, 51, 153, 255);
	
	if (nPlayers == 2) {
		TTF_SizeText(normalFont, "Player 2", &textWidth, &textHeight);
		draw("Player 2", normalFont, ((winWidth / 2 + 150) + winWidth) / 2 - textWidth / 2, winHeight / 2 - textHeight / 2 + 100, 51, 153, 255);
	}
	else {
		TTF_SizeText(normalFont, "You", &textWidth, &textHeight);
		draw("You", normalFont, ((winWidth / 2 + 150) + winWidth) / 2 - textWidth / 2, winHeight / 2 - textHeight / 2 + 100, 51, 153, 255);
	}

	//	End game
	if (game) {
		const char* player = b.getResult();
		const char* sent = (player == "D" ? "Its a draw !!!" : " is the winner !!!");
		if (player == "D") {
			TTF_SizeText(normalFont, sent, &textWidth, &textHeight);
			draw(sent, normalFont, winWidth / 2 - textWidth / 2, winHeight / 2 + 200, 255, 0, 0);
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

//	Tic draw text
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

//	Tic input
void Tic::input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {

		//	Quit button
		if (event.type == SDL_QUIT) {
			running = 0;
			std::cout << "Quitting" << std::endl;
		}

		//	On key press
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				running = 0;
				std::cout << "Quitting" << std::endl;
			}
		}

		//	On mouse click
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