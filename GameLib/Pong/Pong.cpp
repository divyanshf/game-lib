#include "Pong.h"

//	Pong Constructor
Pong::Pong(SDL_Renderer* ren, SDL_Window* win) {
	//	Initialize SDL
	this->ren = ren;
	this->win = win;
	SDL_GetWindowSize(this->win, &winWidth, &winHeight);

	//	Initialize TTF
	TTF_Init();
	titleFont = TTF_OpenFont("Fonts/Chopsic/Chopsic-K6Dp.ttf", 48);
	normalFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 24);
	instructionFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 16);
	head = "PONG";
	int textHeight;
	TTF_SizeText(titleFont, head, NULL, &textHeight);

	//	Initialize audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Failed at Mi_OpenAudio()" << std::endl;
	}
	Mix_VolumeMusic(5);
	bgm = Mix_LoadMUS("Audio/Bgm.wav");
	collideEffect = Mix_LoadWAV("Audio/Collide.wav");
	loseEffect = Mix_LoadWAV("Audio/Lose.wav");
	winnerEffect = Mix_LoadWAV("Audio/Win.wav");
	startEffect = Mix_LoadWAV("Audio/Start.wav");
	wallEffect = Mix_LoadWAV("Audio/Click.wav");

	//	Initialize options
	pongOptions.push_back("1");
	pongOptions.push_back("2");
	pongOption = pongOptions.begin();

	//	Initialize remaining variables
	gameWidth = winWidth - 1;
	gameHeight = (winHeight - 20) - (textHeight + 100);

	//	Game Assets
	ball = new Ball(gameWidth / 2, (winHeight - 20) - gameHeight / 2 - 12);
	player1 = new Paddle(1, (winHeight - 20) - gameHeight / 2 - 50);
	player2 = new Paddle(gameWidth - 1, (winHeight - 20) - gameHeight / 2 - 50);

	std::cout << (player1->getX()) << std::endl;
	std::cout << (player2->getX()) << std::endl;

	//	Set src for ball
	ball->setDest(ball->getX(), ball->getY(), 25, 25);
	ball->setImage("Assets/Pong/ball.png", ren);
	ball->setSource(0, 0, 25, 25);
	
	//	Set src for player 1
	player1->setDest(player1->getX() + 5, player1->getY(), 20, 100);
	player1->setImage("Assets/Pong/paddle.png", ren);
	player1->setSource(0, 0, 20, 100);
	
	//	Set src for player 2
	player2->setDest(player2->getX() - 25, player2->getY(), 20, 100);
	player2->setImage("Assets/Pong/paddle.png", ren);
	player2->setSource(0, 0, 20, 100);

	//	initialize additional variables
	game = 1;
	score1 = score2 = 0;
	running = 1;
}

//	Pong Destructor
Pong::~Pong() {
	Mix_FreeChunk(collideEffect);
	Mix_FreeChunk(loseEffect);
	Mix_FreeChunk(winnerEffect);
	Mix_FreeChunk(wallEffect);
	Mix_FreeMusic(bgm);
	Mix_Quit();
	TTF_CloseFont(titleFont);
	TTF_CloseFont(normalFont);
	TTF_CloseFont(instructionFont);
	TTF_Quit();
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
	TTF_SizeText(titleFont, head, &textWidth, &textHeight);
	draw(head, titleFont, (winWidth / 2) - (textWidth / 2), 10, 255, 255, 0);
	
	draw(textHeight);
	
	//	Scores player1
	TTF_SizeText(normalFont, "Player 1 :", &textWidth, &textHeight);
	draw("Player 1 :", normalFont, 0, 50, 255, 153, 51);
	std::string score = std::to_string(score1);
	draw(score.c_str(), normalFont, 0, 50 + textHeight + 10, 0, 255, 0);

	//	Scores player2
	TTF_SizeText(normalFont, "Player 2 :", &textWidth, &textHeight);
	draw("Player 2 :", normalFont, winWidth - textWidth - 1, 50, 51, 153, 255);
	score = std::to_string(score2);
	draw(score.c_str(), normalFont, winWidth - textWidth - 1, 50 + textHeight + 10, 0, 255, 0);

	//	Instructions
	TTF_SizeText(instructionFont, "Press Q to end", &textWidth, &textHeight);
	draw("Press Q to end", instructionFont, 0, winHeight - textHeight, 191, 191, 63);

	TTF_SizeText(instructionFont, "Press ESC to close", &textWidth, &textHeight);
	draw("Press ESC to close", instructionFont, winWidth - textWidth, winHeight - textHeight, 191, 191, 63);

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	//	Render the items
	SDL_RenderPresent(ren);

	if (!game) {
		running = 0;
		SDL_Delay(3000);
	}

}

void Pong::draw(int titleHeight) {

	int textWidth, textHeight;
	textHeight = titleHeight;
	//	Set the render color 
	SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);

	//	Draw the borders
	SDL_RenderDrawLine(ren, 0, textHeight + 100, winWidth - 1, textHeight + 100);
	SDL_RenderDrawLine(ren, 0, winHeight - 20, winWidth - 1, winHeight - 20);
	SDL_RenderDrawLine(ren, 0, textHeight + 100, 0, winHeight - 20);
	SDL_RenderDrawLine(ren, winWidth - 1, textHeight + 100, winWidth - 1, winHeight - 20);

	//	Buttons
	if (game) {
		if (ball->getDirection() == eDir::STOP) {
			TTF_SizeText(instructionFont, "Press Enter to start!", &textWidth, &textHeight);
			draw("Press Enter to start", instructionFont, winWidth / 2 - textWidth / 2, winHeight / 2 - textHeight / 2 - 50, 255, 0, 0);
		}
	}

	//	Paddles and ball
	if (game) {
		draw(*ball);
		draw(*player1);
		draw(*player2);
	}
	else {
		if (score1 > score2) {
			TTF_SizeText(normalFont, "Player 1 wins !!!", &textWidth, &textHeight);
			draw("Player 1 wins !!!", normalFont, winWidth / 2 - textWidth / 2, winHeight / 2 - textHeight / 2, 255, 153, 51);
		}
		else if (score2 > score1) {
			TTF_SizeText(normalFont, "Player 2 wins !!!", &textWidth, &textHeight);
			draw("Player 2 wins !!!", normalFont, winWidth / 2 - textWidth / 2, winHeight / 2 - textHeight / 2, 51, 153, 255);
		}
		else {
			TTF_SizeText(normalFont, "Its a draw !!", &textWidth, &textHeight);
			draw("Its a draw !!", normalFont, winWidth / 2 - textWidth / 2, winHeight / 2 - textHeight / 2, 255, 0, 0);
		}
	}
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
	if (ballX <= player1X + 21) {
		if (ballY + 12 >= player1Y && ballY + 12 <= player1Y + 100) {
			ball->changeDirection((eDir)((rand() % 3) + 4));
			Mix_PlayChannel(-1, collideEffect, 0);
		}
	}

	//right paddle
	if (ballX + 45 >= player2X) {
		if (ballY + 12 >= player2Y && ballY + 12 <= player2Y + 100) {
			ball->changeDirection((eDir)((rand() % 3) + 1));
			Mix_PlayChannel(-1, collideEffect, 0);
		}
	}

	//bottom wall
	if (ballY >= winHeight - 20 - 25) {
		ball->changeDirection(ball->getDirection() == eDir::DOWNRIGHT ? eDir::UPRIGHT : eDir::UPLEFT);
		Mix_PlayChannel(-1, wallEffect, 0);
	}

	//top wall
	if (ballY <= winHeight - gameHeight - 20) {
		ball->changeDirection(ball->getDirection() == eDir::UPRIGHT ? eDir::DOWNRIGHT : eDir::DOWNLEFT);
		Mix_PlayChannel(-1, wallEffect, 0);
	}

	//right wall 
	if (ballX >= gameWidth - 5 - 20) {
		scoreUp(player1);
		player1->reset();
		player2->reset();
		ball->reset();
		ball->setDest(ball->getX(), ball->getY(), 25, 25);
		player1->setDest(player1->getX() + 5, player1->getY(), 20, 100);
		player2->setDest(player2->getX() - 25, player2->getY(), 20, 100);
		Mix_HaltMusic();
		Mix_PlayChannel(-1, loseEffect, 0);
	}

	//left wall 
	if (ballX <= 0) {
		scoreUp(player2);
		player1->reset();
		player2->reset();
		ball->reset();
		ball->setDest(ball->getX(), ball->getY(), 25, 25);
		player1->setDest(player1->getX() + 5, player1->getY(), 20, 100);
		player2->setDest(player2->getX() - 25, player2->getY(), 20, 100);
		Mix_HaltMusic();
		Mix_PlayChannel(-1, loseEffect, 0);
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
				game = 0;
				Mix_HaltMusic();
				Mix_PlayChannel(-1, winnerEffect, 0);
			}

			//	Player 1 controls
			if (event.key.keysym.sym == SDLK_w) {
				if (player1->getY() > winHeight - 10 - gameHeight) {
					player1->moveUp();
					player1->setDest(player1->getX() + 5, player1->getY(), 20, 100);
				}
			}
			if (event.key.keysym.sym == SDLK_s) {
				if (player1->getY() < winHeight - 100 - 25) {
					player1->moveDown();
					player1->setDest(player1->getX() + 5, player1->getY(), 20, 100);
				}
			}

			//	Player 2 controls
			if (event.key.keysym.sym == SDLK_UP) {
				if (player2->getY() > winHeight - 10 - gameHeight) {
					player2->moveUp();
					player2->setDest(player2->getX() - 25, player2->getY(), 20, 100);
				}
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				if (player2->getY() < winHeight - 100 - 25) {
					player2->moveDown();
					player2->setDest(player2->getX() - 25, player2->getY(), 20, 100);
				}
			}

			//	ball randomize
			if (event.key.keysym.sym == SDLK_RETURN) {
				if (ball->getDirection() == eDir::STOP) {
					ball->randomDirection();
					if (!Mix_PlayingMusic()) {
						Mix_PlayMusic(bgm, -1);
					}
					else if (Mix_PausedMusic()) {
						Mix_ResumeMusic();
					}
				}
			}
		}
	}
}
