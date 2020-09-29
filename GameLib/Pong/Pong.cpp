#include "Pong.h"

//	Pong Constructor
Pong::Pong() {
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
	titleFont = TTF_OpenFont("Fonts/Chopsic/Chopsic-K6Dp.ttf", 48);
	normalFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 24);
	head = "PONG";
	int textHeight;
	TTF_SizeText(titleFont, head, NULL, &textHeight);

	//	Initialize remaining variables
	gameWidth = winWidth - 1;
	gameHeight = (winHeight - 20) - (textHeight + 100);

	//	Game Assets
	ball = new Ball(gameWidth / 2, textHeight + 100 + gameHeight / 2);
	player1 = new Paddle(1, textHeight + 100 + gameHeight / 2);
	player2 = new Paddle(gameWidth - 1, textHeight + 100 + gameHeight / 2);

	std::cout << (player1->getX()) << std::endl;
	std::cout << (player2->getX()) << std::endl;

	//	Set src for ball
	ball->setDest(ball->getX(), ball->getY(), 25, 25);
	ball->setImage("Assets/ball.png", ren);
	ball->setSource(0, 0, 25, 25);
	
	//	Set src for player 1
	player1->setDest(player1->getX() + 5, player1->getY(), 20, 100);
	player1->setImage("Assets/paddle.png", ren);
	player1->setSource(0, 0, 20, 100);
	
	//	Set src for player 2
	player2->setDest(player2->getX() - 25, player2->getY(), 20, 100);
	player2->setImage("Assets/paddle.png", ren);
	player2->setSource(0, 0, 20, 100);

	//	initialize additional variables
	running = 1;
}

//	Pong Destructor
Pong::~Pong() {
	TTF_CloseFont(titleFont);
	TTF_CloseFont(normalFont);
	TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

//	Pong Loop
void Pong::loop() {
	while (running) {
		render();
		input();
	}
}

//	Pong render
void Pong::render() {
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
	TTF_SizeText(titleFont, head, &textWidth, &textHeight);
	draw(head, titleFont, (winWidth / 2) - (textWidth / 2), 10, 255, 255, 0);

	//	Draw the borders
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderDrawLine(ren, 0, textHeight + 100, winWidth - 1, textHeight + 100);
	SDL_RenderDrawLine(ren, 0, winHeight - 20, winWidth - 1, winHeight - 20);
	SDL_RenderDrawLine(ren, 0, textHeight + 100, 0, winHeight - 20);
	SDL_RenderDrawLine(ren, winWidth - 1, textHeight + 100, winWidth - 1, winHeight - 20);

	//	Paddles and ball
	draw(*ball);
	draw(*player1);
	draw(*player2);

	//	Render the items
	SDL_RenderPresent(ren);
}

void Pong::draw(Object obj) {
	SDL_Rect dest = obj.getDest();
	SDL_Rect src = obj.getSource();
	SDL_RenderCopyEx(ren, obj.getTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
}

//	Tic draw text
void Pong::draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b) {
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

void Pong::logic() {

}

void Pong::input() {
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
			if (event.key.keysym.sym == SDLK_w) {
				player1->moveUp();
				player1->setDest(player1->getX() + 5, player1->getY(), 20, 100);
			}
			if (event.key.keysym.sym == SDLK_s) {
				player1->moveDown();
				player1->setDest(player1->getX() + 5, player1->getY(), 20, 100);
			}
			if (event.key.keysym.sym == SDLK_UP) {
				player2->moveUp();
				player2->setDest(player2->getX() - 25, player2->getY(), 20, 100);
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				player2->moveDown();
				player2->setDest(player2->getX() - 25, player2->getY(), 20, 100);
			}
		}
	}
}