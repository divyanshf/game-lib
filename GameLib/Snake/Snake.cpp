#include "Snake.h"

//	Snake constructor
Snake::Snake(SDL_Renderer* ren, SDL_Window* win, std::string username) {
	//	Initialize SDL
	this->ren = ren;
	this->win = win;
	this->username = username;
	SDL_GetWindowSize(this->win, &winWidth, &winHeight);
	srand(time(NULL));

	//	Initialize Audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Failed at Mi_OpenAudio()" << std::endl;
	}
	Mix_Volume(-1, 100);
	Mix_VolumeMusic(5);
	bgm = Mix_LoadMUS("Audio/Bgm.wav");
	loseEffect = Mix_LoadWAV("Audio/Lose.wav");
	winnerEffect = Mix_LoadWAV("Audio/Win.wav");
	gulpEffect = Mix_LoadWAV("Audio/gulp.wav");
	volume.setDest(winWidth - 25, 0, 25, 25);
	volume.setImage("Assets/GameImage/Volume.png", ren);
	volume.setSource(0, 0, 50, 50);

	//	Initialize TTF
	TTF_Init();
	head = "SNAKE";
	titleFont = TTF_OpenFont("Fonts/Chopsic/Chopsic-K6Dp.ttf", 48);
	normalFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 24);
	instructionFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 18);

	//	Snake
	sShape.setImage("Assets/Snake/snake.png", ren);
	sShape.setSource(0, 0, 25, 25);
	direction = sDir::STOP;

	//	Fruit
	fruit.setImage("Assets/Snake/fruit.png", ren);
	fruit.setSource(0, 0, 25, 25);

	//	BestScore
	std::string scoreFileName = "Assets/User/" + username + "/Snake.txt";
	iScoreFile.open(scoreFileName.c_str());
	if (!iScoreFile) {
		oScoreFile.open(scoreFileName.c_str());
		bestScore = 0;
		std::string tmpString = std::to_string(bestScore);
		oScoreFile << tmpString.c_str() << std::endl;
		oScoreFile.close();
		std::cout << "new score" << std::endl;
	}
	else {
		std::string tmpScore;
		iScoreFile >> tmpScore;
		bestScore = std::stoi(tmpScore);
		iScoreFile.close();
		std::cout << "took score : " << bestScore << std::endl;
	}

	//	Initialize other variables
	FPS = 60;
	frameDelay = 1000 / FPS; 
	isMute = 0;
	grids = 15;
	mainX = winWidth / 2 - (grids * 25) / 2;
	mainY = winHeight / 2 - (grids * 25) / 2;
	posX = mainX + 2 * 25;
	posY = mainY;
	fruitX = rand() % grids * 25 + mainX;
	fruitY = rand() % grids * 25 + mainY;
	nTail = 3;
	score = 0;
	game = 0;
	running = 1;

	//	Snake vector
	for (int i = 0; i < nTail; i++) {
		if (i == 0) {
			Object headShape;
			headShape.setImage("Assets/Snake/head.png", ren);
			headShape.setSource(0, 0, 25, 25);
			snake.push_back(headShape);
		}
		else {
			snake.push_back(sShape);
		}
		x.push_back(posX - i * 25);
		y.push_back(posY);
	}

	for (int i = 0; i < nTail; i++) {
		snake[i].setDest(x[i], y[i], 25, 25);
	}
}

//	Snake destructor
Snake::~Snake() {
	Mix_FreeChunk(loseEffect);
	Mix_FreeChunk(winnerEffect);
	Mix_FreeChunk(gulpEffect);
	Mix_FreeMusic(bgm);
	Mix_Quit();
	TTF_CloseFont(titleFont);
	TTF_CloseFont(normalFont);
	TTF_Quit();
}

//	Snake loop
void Snake::loop() {
	while (running) {
		frameStart = SDL_GetTicks();

		render();
		input();
		logic();

		SDL_Delay(150);

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	Mix_HaltMusic();
	SDL_Delay(3000);
}

//	Snake render
void Snake::render() {
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

	//	Instructions
	TTF_SizeText(instructionFont, "Press ESC to close", &textWidth, &textHeight);
	draw("Press ESC to close", instructionFont, winWidth - textWidth, winHeight - textHeight, 191, 191, 63);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	for (int i = 0; i < grids + 1; i++) {	//	vertical grid lines
		SDL_RenderDrawLine(ren, winWidth / 2 + i * 25 - (grids * 25) / 2, winHeight / 2 - (grids * 25) / 2, winWidth / 2 + i * 25 - (grids * 25) / 2, winHeight / 2 + grids * 25 - (grids * 25) / 2);
	}
	for (int i = 0; i < grids + 1; i++) {	//	horizontal grid lines
		SDL_RenderDrawLine(ren, winWidth / 2 - (grids * 25) / 2, winHeight / 2 + i * 25 - (grids * 25) / 2, winWidth / 2 + grids * 25 - (grids * 25) / 2, winHeight / 2 + i * 25 - (grids * 25) / 2);
	}

	//	Snake
	for (int i = 0; i < nTail; i++) {
		draw(snake[i]);
	}
	
	//	Fruit
	fruit.setDest(fruitX, fruitY, 25, 25);
	draw(fruit);

	//	Volume
	draw(volume);

	//	score
	std::string scoreString = "Score : " + std::to_string(score);
	TTF_SizeText(normalFont, scoreString.c_str(), &textWidth, &textHeight);
	draw(scoreString.c_str(), normalFont, winWidth / 2 - textWidth / 2, winHeight / 2 + grids / 2 * 25 + 2 * textHeight, 0, 255, 0);

	//	bestScore
	std::string bestScoreString = "Best Score : " + std::to_string(bestScore);
	TTF_SizeText(normalFont, bestScoreString.c_str(), &textWidth, &textHeight);
	draw(bestScoreString.c_str(), normalFont, 0, 0, 255, 255, 0);

	//	Username
	int tmpW, tmpH;
	TTF_SizeText(instructionFont, "User : ", &tmpW, &tmpH);
	draw("User : ", instructionFont, 0, winHeight - tmpH, 255, 255, 0);
	draw(username.c_str(), instructionFont, tmpW, winHeight - tmpH, 0, 255, 0);

	//	Press enter
	if (game == 0) {
		TTF_SizeText(instructionFont, "Press Enter to play !", &textWidth, &textHeight);
		draw("Press Enter to play !", instructionFont, winWidth / 2 - textWidth / 2, mainY + grids * 25 + textHeight, 255, 0, 0);
	}

	//	Render the items
	SDL_RenderPresent(ren);
}

void Snake::draw(Object obj) {
	SDL_Rect dest = obj.getDest();
	SDL_Rect src = obj.getSource();
	SDL_RenderCopyEx(ren, obj.getTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
}

//	Draw text on screen
void Snake::draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b) {
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

//	Snake logic
void Snake::logic() {
	switch (direction) {
	case sDir::STOP:
		break;
	case sDir::UP:
		posY -= 25;
		if (posY == mainY - 25) {
			running = 0;
			Mix_PlayChannel(-1, loseEffect, 0);
		}
		break;
	case sDir::DOWN:
		posY += 25;
		if (posY == mainY + (grids * 25)) {
			running = 0;
			Mix_PlayChannel(-1, loseEffect, 0);
		}
		break;
	case sDir::LEFT:
		posX -= 25;
		if (posX == mainX - 25) {
			running = 0;
			Mix_PlayChannel(-1, loseEffect, 0);
		}
		break;
	case sDir::RIGHT:
		posX += 25;
		if (posX == mainX + (grids * 25)) {
			running = 0;
			Mix_PlayChannel(-1, loseEffect, 0);
		}
		break;
	}

	vecObjectPos tmpX;
	vecObjectPos tmpY;
	tmpX = x;
	tmpY = y;
	
	if (posX == fruitX && posY == fruitY) {
		fruitX = rand() % grids * 25 + mainX;
		fruitY = rand() % grids * 25 + mainY;
		snake.push_back(sShape);
		x.push_back(tmpX[nTail - 1]);
		y.push_back(tmpY[nTail - 1]);
		nTail++;
		score += 5;
		if (score > bestScore) {
			bestScore = score;
			std::string tmpScore = std::to_string(bestScore);
			std::string scoreFileName = "Assets/User/" + username + "/Snake.txt";
			oScoreFile.open(scoreFileName.c_str());
			if (oScoreFile.is_open()) {
				std::string tmpString = std::to_string(bestScore);
				oScoreFile << tmpString.c_str() << std::endl;
				oScoreFile.close();
				std::cout << "Updated bestScore" << std::endl;
			}
			else {
				std::cout << "Couldn't update bestScore" << std::endl;
			}
		}
		Mix_PlayChannel(-1, gulpEffect, 0);
	}

	if (game) {
		for (int i = 0; i < nTail; i++) {
			x[i] = i == 0 ? posX : tmpX[i - 1];
			y[i] = i == 0 ? posY : tmpY[i - 1];
			snake[i].setDest(x[i], y[i], 25, 25);
		}
	}

	for (int i = 1; i < nTail; i++) {
		if (x[i] == posX && y[i] == posY) {
			running = 0;
			Mix_PlayChannel(-1, loseEffect, 0);
		}
	}
}

//	Snake input
void Snake::input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {

		//	Quit button
		if (event.type == SDL_QUIT) {
			running = 0;
			std::cout << "Quitting" << std::endl;
		}

		//	On mouse click
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.motion.x > winWidth - 30 && event.motion.x < winWidth) {
				if (event.motion.y > 0 && event.motion.y < 30) {
					if (isMute) {
						Mix_Volume(-1, 100);
						Mix_VolumeMusic(5);
						volume.setSource(0, 0, 50, 50);
					}
					else {
						Mix_Volume(-1, 0);
						Mix_VolumeMusic(0);
						volume.setSource(50, 0, 50, 50);
					}
					isMute = !isMute;
				}
			}
		}

		//	On key press
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				running = 0;
				Mix_PlayChannel(-1, winnerEffect, 0);
			}

			//	Movement
			if (direction != sDir::STOP) {
				if (event.key.keysym.sym == SDLK_UP) {
					if (direction != sDir::DOWN)
						direction = sDir::UP;
				}
				else if (event.key.keysym.sym == SDLK_DOWN) {
					if (direction != sDir::UP)
						direction = sDir::DOWN;
				}
				else if (event.key.keysym.sym == SDLK_LEFT) {
					if (direction != sDir::RIGHT)
						direction = sDir::LEFT;
				}
				else if (event.key.keysym.sym == SDLK_RIGHT) {
					if (direction != sDir::LEFT)
						direction = sDir::RIGHT;
				}
			}
			else {
				if (event.key.keysym.sym == SDLK_RETURN) {
					direction = sDir::RIGHT;
					game = 1;
					if (!Mix_PlayingMusic()) {
						Mix_PlayMusic(bgm, -1);
					}
					else if (Mix_PausedMusic()) {
						Mix_ResumeMusic();
					}
				}
			}

			//	Mute toggle
			if (event.key.keysym.sym == SDLK_m) {
				if (isMute) {
					Mix_Volume(-1, 100);
					Mix_VolumeMusic(5);
					volume.setSource(0, 0, 50, 50);
				}
				else {
					Mix_Volume(-1, 0);
					Mix_VolumeMusic(0);
					volume.setSource(50, 0, 50, 50);
				}
				isMute = !isMute;
			}
		}
	}
}
