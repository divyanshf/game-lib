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
	score1 = score2 = 0;
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
		logic();
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
	int ballX = ball->getX();
	int ballY = ball->getY();
	int player1X = player1->getX();
	int player2X = player2->getX();
	int player1Y = player1->getY();
	int player2Y = player2->getY();

	//left paddle
	if (ballX == player1X + 21) {
		if (ballY >= player1Y && ballY <= player1Y - 25) {
			ball->changeDirection((eDir)((rand() % 3) + 4));
		}
	}

	//right paddle
	if (ballX + 26 == player2X) {
		if (ballY >= player2Y && ballY <= player2Y - 25) {
			ball->changeDirection((eDir)((rand() % 3) + 1));
		}
	}

	//bottom wall
	if (ballY >= winHeight - 20 - 25) {
		ball->changeDirection(ball->getDirection() == eDir::DOWNRIGHT ? eDir::UPRIGHT : eDir::UPLEFT);
	}
	//top wall
	if (ballY <= winHeight - 20 - gameHeight) {
		ball->changeDirection(ball->getDirection() == eDir::UPRIGHT ? eDir::DOWNRIGHT : eDir::DOWNLEFT);
	}

	//right wall 
	if (ballX >= gameWidth - 5 - 20) {
		//scoreUp(player1);
		ball->changeDirection(ball->getDirection() == eDir::UPRIGHT ? eDir::UPLEFT : eDir::DOWNLEFT);
	}
	//left wall 
	if (ballX <= 0) {
		//scoreUp(player2);
		ball->changeDirection(ball->getDirection() == eDir::UPLEFT ? eDir::UPRIGHT : eDir::DOWNRIGHT);
	}
}

void Pong::scoreUp(Paddle* player) {
	if (player == player1) {
		score1 += 10;
	}
	else if (player == player2) {
		score2 += 10;
	}
	ball->reset();
	player1->reset();
	player2->reset();
}

void Pong::input() {
	ball->move();
	ball->setDest(ball->getX(), ball->getY(), 25, 25);

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

			//	Player 1 controls
			if (event.key.keysym.sym == SDLK_w) {
				if (player1->getY() > winHeight - gameHeight - 15) {
					player1->moveUp();
					player1->setDest(player1->getX() + 5, player1->getY(), 20, 100);
				}
			}
			if (event.key.keysym.sym == SDLK_s) {
				if (player1->getY() < winHeight - 100 - 23) {
					player1->moveDown();
					player1->setDest(player1->getX() + 5, player1->getY(), 20, 100);
				}
			}

			//	Player 2 controls
			if (event.key.keysym.sym == SDLK_UP) {
				if (player2->getY() > winHeight - gameHeight - 15) {
					player2->moveUp();
					player2->setDest(player2->getX() - 25, player2->getY(), 20, 100);
				}
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				if (player2->getY() < winHeight - 100 - 23) {
					player2->moveDown();
					player2->setDest(player2->getX() - 25, player2->getY(), 20, 100);
				}
			}

			//	ball randomize
			if (ball->getDirection() == eDir::STOP) {
				ball->randomDirection();
			}
		}
	}
}