#include "Hang.h"

Hang::Hang(SDL_Renderer* ren, SDL_Window* win) {
	//	Initialize SDL
	this->ren = ren;
	this->win = win;
	SDL_GetWindowSize(this->win, &winWidth, &winHeight);

	//	Initialize TTF
	TTF_Init();
	SDL_StartTextInput();

	//	Initialize audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Failed at Mi_OpenAudio()" << std::endl;
	}
	Mix_VolumeMusic(5);
	bgm = Mix_LoadMUS("Audio/HangBgm.wav");
	clickEffect = Mix_LoadWAV("Audio/Click.wav");
	thankEffect = Mix_LoadWAV("Audio/Thanks.wav");
	loseEffect = Mix_LoadWAV("Audio/Lose.wav");
	winnerEffect = Mix_LoadWAV("Audio/Win.wav");
	screamEffect = Mix_LoadWAV("Audio/Scream3.wav");
	errorEffect = Mix_LoadWAV("Audio/Error.wav");

	//	Initialize title and fonts
	head = "HANGMAN";
	titleFont = TTF_OpenFont("Fonts/Chopsic/Chopsic-K6Dp.ttf", 48);
	normalFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 24);
	playerFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 56);
	instructionFont = TTF_OpenFont("Fonts/Montserrat/MontserratLight-ywBvq.ttf", 16);
	
	//	Open file containing words and select a random one
	srand(time(NULL));
	file.open("Assets/Hangman/hangwords.txt");
	while (file >> word) {
		std::transform(word.begin(), word.end(), word.begin(), ::toupper);
		if (word.length() <= 10) {
			words.push_back(word);
		}
	}
	word = words[(rand() % 61333)];

	//	Chances
	chances = 5;

	//	Initialize remaining variables
	FPS = 60;
	frameDelay = 1000 / FPS;
	anim.setDest(1, winHeight - 550, 500, 500);
	anim.setImage("Assets/Hangman/Anim.png", ren);
	anim.setSource(0, 0, 500, 500);
	winner = -1;
	guess = "";
	game = 1;
	running = 1;
}
Hang::~Hang() {
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
void Hang::loop() {
	Mix_PlayMusic(bgm, -1);
	while (running) {
		frameStart = SDL_GetTicks();

		render();
		input();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	Mix_HaltMusic();
	if (game) {
		game = 1;
		if (winner == 0) {
			game = 3;
			Mix_PlayChannel(-1, loseEffect, 0);
		}
		else {
			Mix_PlayChannel(-1, winnerEffect, 0);
		}
		FPS = 1;
		while (game) {
			frameStart = SDL_GetTicks();

			render();
			updateAnimation();

			frameTime = SDL_GetTicks() - frameStart;

			if (frameDelay > frameTime) {
				SDL_Delay(frameDelay - frameTime);
			}
		}
		SDL_Delay(4000);
	}
}
void Hang::updateAnimation() {
	game--;
	SDL_Rect tmpRect;
	tmpRect = anim.getSource();
	if (winner) {
		if (game == 0) {
			Mix_PlayChannel(-1, thankEffect, 0);
		}
		anim.setSource(tmpRect.x + 500, tmpRect.y + 500, 500, 500);
	}
	else {
		Mix_PlayChannel(-1, screamEffect, 0);
		anim.setSource(tmpRect.x + 500, tmpRect.y, 500, 500);
	}
}
void Hang::render() {
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

	SDL_SetRenderDrawColor(ren, 135, 206, 235, 255);
	SDL_RenderDrawLine(ren, 1, winHeight - 50, 501, winHeight - 50);
	SDL_RenderDrawLine(ren, 1, winHeight - 600, 501, winHeight - 600);
	SDL_RenderDrawLine(ren, 1, winHeight - 50, 1, winHeight - 600);
	SDL_RenderDrawLine(ren, 501, winHeight - 50, 501, winHeight - 600);
	
	SDL_Rect animRect;
	animRect.w = 500;
	animRect.h = 550;
	animRect.x = 1;
	animRect.y = winHeight - 600;
	SDL_RenderFillRect(ren, &animRect);

	draw();

	draw(anim);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

	SDL_RenderPresent(ren);
}
void Hang::draw() {
	int textHeight, textWidth;
	int spaceWidth, spaceHeight;
	TTF_SizeText(normalFont, "A", &spaceWidth, &spaceHeight);
	int totalWidth = 26 * spaceWidth;
	std::string s;
	int i = 0;
	int j = 250;

	//	Set render draw color
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

	//	All the letters
	SDL_Rect availRect;
	availRect.x = 501 + (winWidth - 501 - totalWidth) / 2 - spaceWidth;
	availRect.y = j - 5;
	availRect.w = totalWidth + 10 + spaceWidth;
	availRect.h = spaceWidth * 2 + 10 + 50;
	SDL_RenderDrawRect(ren, &availRect);
	for (char ch = 'A'; ch <= 'Z'; ch++) {
		s = ch;
		if (guess.find(ch) != std::string::npos && word.find(ch) == std::string::npos) {
			draw(s.c_str(), normalFont, 501 + (winWidth - 501 - totalWidth) / 2 + i * 2 * spaceWidth, j, 255, 0, 0);
		}
		else if (guess.find(ch) != std::string::npos && word.find(ch) != std::string::npos) {
			draw(s.c_str(), normalFont, 501 + (winWidth - 501 - totalWidth) / 2 + i * 2 * spaceWidth, j, 0, 255, 0);
		}
		else {
			draw(s.c_str(), normalFont, 501 + (winWidth - 501 - totalWidth) / 2 + i * 2 * spaceWidth, j, 255, 255, 255);
		}
		i++;
		if (ch == 'M') {
			i = 0;
			j += 50;
		}
	}

	//	The random word
	totalWidth = word.length() * 2 * spaceWidth;
	SDL_Rect randRect;
	randRect.x = 496 + (winWidth - 501 - totalWidth) / 2 - spaceWidth;
	randRect.y = 395;
	randRect.w = totalWidth + 10 + spaceWidth;
	randRect.h = spaceHeight + 10;
	SDL_RenderDrawRect(ren, &randRect);
	if (winner == 0) {
		for (i = 0; i < word.length(); i++) {
			s = word[i];
			int r = 255, g = 255, b = 255;
			if (guess.find(word[i]) == std::string::npos) {
				r = 0;
				b = 0;
			}
			draw(s.c_str(), normalFont, 501 + (winWidth - 501 - totalWidth) / 2 + i * 2 * spaceWidth, 400, r, g, b);
		}
	}
	else {
		for (i = 0; i < word.length(); i++) {
			if (guess.find(word[i]) != std::string::npos) {
				s = word[i];
			}
			else {
				s = "_";
			}
			draw(s.c_str(), normalFont, 501 + (winWidth - 501 - totalWidth) / 2 + i * 2 * spaceWidth, 400, 255, 255, 255);
		}
	}

	//	On getting a winner or not
	SDL_Rect winRect;
	if (winner == 0) {
		TTF_SizeText(normalFont, "You lost !!!", &textWidth, &textHeight);
		winRect.x = 496 + (winWidth - 501 - textWidth) / 2;
		winRect.y = 495;
		winRect.w = textWidth + 10;
		winRect.h = textHeight + 10;
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
		SDL_RenderDrawRect(ren, &winRect);
		draw("You lost !!!", normalFont, 501 + (winWidth - 501 - textWidth) / 2, 500, 255, 0, 0);
	}
	else if (winner == 1) {
		TTF_SizeText(normalFont, "You won !!!", &textWidth, &textHeight);
		winRect.x = 496 + (winWidth - 501 - textWidth) / 2;
		winRect.y = 495;
		winRect.w = textWidth + 10;
		winRect.h = textHeight + 10; 
		SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
		SDL_RenderDrawRect(ren, &winRect);
		draw("You won !!!", normalFont, 501 + (winWidth - 501 - textWidth) / 2, 500, 0, 255, 0);
	}
	else {
		std::string moves = "Wrong Moves Left : " + std::to_string(chances);
		TTF_SizeText(normalFont, moves.c_str(), &textWidth, &textHeight);
		draw(moves.c_str(), normalFont, 501 + (winWidth - 501 - textWidth) / 2, 150, 0, 0, 255);
	}
	
	//	Instructions
	TTF_SizeText(instructionFont, "Press ESC to close", &textWidth, &textHeight);
	draw("Press ESC to close", instructionFont, winWidth - textWidth, winHeight - textHeight, 191, 191, 63);

}
void Hang::draw(Object obj) {
	SDL_Rect dest = obj.getDest();
	SDL_Rect src = obj.getSource();
	SDL_RenderCopyEx(ren, obj.getTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
}
void Hang::draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b) {
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
int Hang::getWinner() {
	for (int i = 0; i < word.length(); i++) {
		if (guess.find(word[i]) == std::string::npos) {
			if (chances < 0) {
				anim.setSource(500, 0, 500, 500);
				return 0;
				chances++;
			}
			else {
				return -1;
			}
		}
	}
	anim.setSource(500, 500, 500, 500);
	return 1;
}
void Hang::input() {
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
		}

		if (event.type == SDL_TEXTINPUT && chances > -1) {
			std::string ch = event.text.text;
			if (ch.length() == 1) {
				std::transform(ch.begin(), ch.end(), ch.begin(), ::toupper);
				if ((guess.find(ch)) == std::string::npos) {
					Mix_PlayChannel(-1, clickEffect, 0);
					guess += ch;

					if (word.find(ch) == std::string::npos) {
						Mix_PlayChannel(-1, errorEffect, 0);
						chances--;
					}

					winner = getWinner();
					if (winner != -1) {
						running = 0;
					}
				}
			}
		}
	}
}