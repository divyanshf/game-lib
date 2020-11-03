#include "Flappy.h"

Flappy::Flappy(SDL_Renderer* ren, SDL_Window* win, std::string username) {
	//	Initialize SDL
	this->ren = ren;
	this->win = win;
	this->username = username;
	SDL_GetWindowSize(this->win, &winWidth, &winHeight);

	//	Initialize TTF
	TTF_Init();
	SDL_StartTextInput();

	//	Initialize audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Failed at Mi_OpenAudio()" << std::endl;
	}
	Mix_Volume(-1, 100);
	Mix_VolumeMusic(5);
	bgm = Mix_LoadMUS("Resources/Audio/HangBgm.wav");
	clickEffect = Mix_LoadWAV("Resources/Audio/Click.wav");
	thankEffect = Mix_LoadWAV("Resources/Audio/Thanks.wav");
	loseEffect = Mix_LoadWAV("Resources/Audio/Lose.wav");
	winnerEffect = Mix_LoadWAV("Resources/Audio/Win.wav");
	screamEffect = Mix_LoadWAV("Resources/Audio/Scream3.wav");
	errorEffect = Mix_LoadWAV("Resources/Audio/Error.wav");
	volume.setDest(winWidth - 25, 0, 25, 25);
	volume.setImage("Resources/Assets/GameImage/Volume.png", ren);
	volume.setSource(0, 0, 50, 50);
	bird.setImage("Resources/Assets/Flappy/flappy.png", ren);
	bird.setSource(0, 0, 300, 200);

	//	Initialize title and fonts
	head = "FLY AWAY";
	titleFont = TTF_OpenFont("Resources/Fonts/Chopsic/Chopsic-K6Dp.ttf", 48);
	normalFont = TTF_OpenFont("Resources/Fonts/Montserrat/MontserratLight-ywBvq.ttf", 24);
	playerFont = TTF_OpenFont("Resources/Fonts/Montserrat/MontserratLight-ywBvq.ttf", 56);
	instructionFont = TTF_OpenFont("Resources/Fonts/Montserrat/MontserratLight-ywBvq.ttf", 16);

	//	randomization
	srand(time(NULL));

	//	File
	scoreFileName = "Resources/User/" + username + "/Flappy.txt";
	scoreFile.open(scoreFileName, std::fstream::in);
	if (!scoreFile) {
		scoreFile.open(scoreFileName, std::fstream::out);
		bestScore = 0;
		std::string tmpString = std::to_string(bestScore);
		scoreFile << tmpString.c_str() << std::endl;
		scoreFile.close();
	}
	else {
		std::string tmpScore;
		scoreFile >> tmpScore;
		bestScore = std::stoi(tmpScore);
		scoreFile.close();
	}

	//	Initialize remaining variables
	FPS = 2;
	frameDelay = 1000 / FPS;
	isMute = false;
	game = 1;
	running = 1;
	score = 0;
	gravity = 1;
	velocity = 4;
	lift = -100;
	liftCounter = 0;
	isLift = 0;
	birdHeight = winHeight / 2 - 22.5;
	birdHeight -= birdHeight % 4;
	bird.setDest(winWidth / 2 - 25, birdHeight, 50, 45);
	pipeWidth = 120;
	pipeGap = 200;
	pipeSpeed = 1;
}

//	Hang Destructor
Flappy::~Flappy() {
	Mix_FreeChunk(clickEffect);
	Mix_FreeChunk(loseEffect);
	Mix_FreeChunk(winnerEffect);
	Mix_FreeChunk(screamEffect);
	Mix_FreeChunk(thankEffect);
	Mix_FreeMusic(bgm);
	Mix_Quit();
	TTF_CloseFont(titleFont);
	TTF_CloseFont(normalFont);
	TTF_CloseFont(playerFont);
	SDL_StopTextInput();
	TTF_Quit();
}

//	Init pipes
void Flappy::addPipes() {
	pipe tmpPipe;
	tmpPipe.x = winWidth;
	int tmpTop = rand() % (winHeight / 2) + winHeight / 4;
	tmpTop -= tmpTop % 4;
	int tmpBottom = tmpTop + pipeGap;
	tmpPipe.top.x = tmpPipe.bottom.x = tmpPipe.x;
	tmpPipe.top.w = tmpPipe.bottom.w = pipeWidth;
	tmpPipe.top.y = 0;
	tmpPipe.top.h = tmpTop;
	tmpPipe.bottom.y = tmpBottom;
	tmpPipe.bottom.h = winHeight - tmpBottom;

	pipes.push_back(tmpPipe);
}

//	Hang main loop
void Flappy::loop() {
	Mix_PlayMusic(bgm, -1);
	while (running) {
		frameStart = SDL_GetTicks();

		render();
		input();
		update();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	Mix_HaltMusic();
	Mix_PlayChannel(-1, loseEffect, 0);
	SDL_Delay(3000);
}

void Flappy::render() {
	SDL_RenderClear(ren);

	int textWidth, textHeight;

	
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_Rect rect;
	rect.w = winWidth;
	rect.h = winHeight;
	rect.x = 0;
	rect.y = 0;
	SDL_RenderFillRect(ren, &rect);

	//	Pipes
	SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
	for (auto i = pipes.begin(); i != pipes.end(); i++) {
		SDL_RenderFillRect(ren, &((*i).top));
		SDL_RenderFillRect(ren, &((*i).bottom));
	}

	//	Head Text
	TTF_SizeText(titleFont, head, &textWidth, &textHeight);
	draw(head, titleFont, (winWidth / 2) - (textWidth / 2), 10, 255, 255, 0);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

	//	Username
	int tmpW, tmpH;
	TTF_SizeText(instructionFont, "User : ", &tmpW, &tmpH);
	draw("User : ", instructionFont, 0, winHeight - tmpH, 255, 255, 0);
	draw(username.c_str(), instructionFont, tmpW, winHeight - tmpH, 0, 255, 0);

	//	Bird
	draw(bird);

	if (pipes.size() == 0) {
		TTF_SizeText(instructionFont, "Press Enter to start", &textWidth, &textHeight);
		draw("Press Enter to start", instructionFont, winWidth / 2 - textWidth / 2, winHeight / 2 - textHeight / 2 + 50, 0, 255, 0);
	}

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	if (!game) {
		TTF_SizeText(instructionFont, "GAME OVER!!!", &textWidth, &textHeight);
		SDL_Rect endRect;
		endRect.x = winWidth / 2 - textWidth / 2;
		endRect.y = winHeight / 2 - textHeight / 2;
		endRect.w = textWidth;
		endRect.h = textHeight;
		SDL_RenderFillRect(ren, &endRect);
		draw("GAME OVER!!!", instructionFont, winWidth / 2 - textWidth / 2, winHeight / 2 - textHeight / 2, 255, 0, 0);
		running = 0;
	}

	//	score
	std::string targetScore = "Score : " + std::to_string(score);
	TTF_SizeText(instructionFont, targetScore.c_str(), &textWidth, &textHeight);
	SDL_Rect scoreRect;
	scoreRect.x = 0;
	scoreRect.y = 50;
	scoreRect.w = textWidth;
	scoreRect.h = textHeight;
	SDL_RenderFillRect(ren, &scoreRect);
	draw(targetScore.c_str(), instructionFont, 0, 50, 0, 0, 255);

	//	Best
	std::string bestTargetScore = "Best : " + std::to_string(bestScore);
	TTF_SizeText(instructionFont, bestTargetScore.c_str(), &textWidth, &textHeight);
	SDL_Rect bestRect;
	bestRect.x = 0;
	bestRect.y = 0;
	bestRect.w = textWidth;
	bestRect.h = textHeight;
	SDL_RenderFillRect(ren, &bestRect);
	draw(bestTargetScore.c_str(), instructionFont, 0, 0, 0, 0, 255);

	//	Instruction
	TTF_SizeText(instructionFont, "Press ESC to close", &textWidth, &textHeight);
	draw("Press ESC to close", instructionFont, winWidth - textWidth, winHeight - textHeight, 191, 191, 63);

	//	Volume
	draw(volume);

	SDL_RenderPresent(ren);
}

void Flappy::draw(Object obj) {
	SDL_Rect dest = obj.getDest();
	SDL_Rect src = obj.getSource();
	SDL_RenderCopyEx(ren, obj.getTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
}

//	Flappy draw text
void Flappy::draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b) {
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

//	Flappy update
void Flappy::update() {
	//	Bird
	if (pipes.size() != 0) {
		if (isLift) {
			liftCounter++;
			velocity = -(gravity * 4);
			if (liftCounter * gravity * 4 == abs(lift)) {
				isLift = 0;
				liftCounter = 0;
				velocity = gravity * 4;
				updateAnimation();
			}
		}
		else {
			velocity = gravity * 4;
		}
		birdHeight = birdHeight + velocity;
		bird.setDest(winWidth / 2 - 25, birdHeight, 50, 45);
		if (birdHeight >= winHeight - 45) {
			velocity = 0;
			birdHeight = winHeight - 45;
			game = 0;
		}
		if (birdHeight <= 0) {
			velocity = 0;
			birdHeight = 0;
			game = 0;
		}
	}
	else {
		updateAnimation();
	}

	//	Pipes
	int increment = 0;
	int remove = 0;
	int birdPosX = bird.getDX();
	int birdPosY = bird.getDY();
	for (auto i = pipes.begin(); i != pipes.end(); i++) {
		(*i).x -= pipeSpeed;
		(*i).top.x = (*i).x;
		(*i).bottom.x = (*i).x;

		//	pipe stuff
		if ((*i).x == winWidth / 2 + 1 * pipeWidth) {
			increment++;
		}
		else if ((*i).x <= -(pipeWidth + 20)) {
			remove = 1;
		}

		//	score
		if ((*i).x + pipeWidth == winWidth / 2 - 25) {
			score += 10;
		}

		//	lose game
		if ((*i).x == birdPosX + 50) {	//	from the front
			if (birdPosY <= (*i).top.h || birdPosY >= (*i).bottom.y) {
				std::cout << "from the front" << std::endl;
				game = 0;
			}
		}
		if (birdPosX + 50 >= (*i).x && birdPosX <= (*i).x + pipeWidth) {	//	in the middle
			if (birdPosY <= (*i).top.h || birdPosY + 45 >= (*i).bottom.y) {
				std::cout << "in the middle" << std::endl;
				game = 0;
			}
		}
	}
	
	if (bestScore < score) {
		bestScore = score;
		scoreFile.open(scoreFileName, std::fstream::out);
		std::string tmpString = std::to_string(bestScore);
		scoreFile << tmpString.c_str() << std::endl;
		scoreFile.close();
	}

	while (increment--) {
		addPipes();
	}
	if (remove) {
		pipes.erase(pipes.begin());
		std::cout << pipes.size() << std::endl;
	}
}

//	Flappy update animation
void Flappy::updateAnimation() {
	SDL_Rect tmpRect = bird.getSource();
	if (tmpRect.x == 0) {
		bird.setSource(300, 0, 300, 200);
	}
	else {
		bird.setSource(0, 0, 300, 200);
	}
}

//	Flappy input
void Flappy::input() {
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
				game = 0;
			}
			if (event.key.keysym.sym == SDLK_RETURN) {
				if (pipes.size() == 0) {
					addPipes();
					FPS = 60;
					frameDelay = 1000 / FPS;
				}
			}
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
			if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE) {
				if (pipes.size() != 0) {
					if (!isLift) {
						isLift = 1;
						updateAnimation();
					}
				}
			}
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
	}
}