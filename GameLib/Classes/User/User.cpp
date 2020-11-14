#include "User.h"

//	User Constructor
User::User(SDL_Renderer* ren, SDL_Window* win) {
	this->ren = ren;
	this->win = win;
	SDL_GetWindowSize(this->win, &winWidth, &winHeight);

	//	Initialize Audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Failed at Mi_OpenAudio()" << std::endl;
	}
	clickEffect = Mix_LoadWAV("Resources/Audio/Click.wav");
	startEffect = Mix_LoadWAV("Resources/Audio/Start.wav");

	//	Initialize TTF
	TTF_Init();
	head = "GameLib";
	headFont = TTF_OpenFont("Resources/Fonts/Chopsic/Chopsic-K6Dp.ttf", 48);
	normalFont = TTF_OpenFont("Resources/Fonts/Montserrat/MontserratLight-ywBvq.ttf", 24);
	userFont = TTF_OpenFont("Resources/Fonts/Mono/VeraMono-Bold.ttf", 24);
	instructionFont = TTF_OpenFont("Resources/Fonts/Montserrat/MontserratLight-ywBvq.ttf", 16);

	//	forbidden characters
	forbid.push_back('*');
	forbid.push_back('.');
	forbid.push_back('"');
	forbid.push_back('/');
	forbid.push_back('\\');
	forbid.push_back('[');
	forbid.push_back(']');
	forbid.push_back(':');
	forbid.push_back(';');
	forbid.push_back('|');
	forbid.push_back(',');

	//	Initialize variables
	running = 1;
	quit = 0;
	username = "";
	error = false;
	putPassword = false;
}

//	User Destructor
User::~User() {
	Mix_FreeChunk(clickEffect);
	Mix_FreeChunk(startEffect);
	Mix_Quit();
	TTF_CloseFont(headFont);
	TTF_CloseFont(normalFont);
	TTF_CloseFont(userFont);
	TTF_Quit();
}
//	User error
int User::checkError() {
	for (int i = 0; i < username.length(); i++) {
		for (int j = 0; j < 11; j++) {
			if (username[i] == forbid[j])
				return 0;
		}
	}
	return 1;
}

//	Encrypt password
void User::encrypt() {
	std::string tmpPassFile = "Resources/User/" + username + "/pass.txt";
	const char* passFileName = tmpPassFile.c_str();
	passFile.open(passFileName, std::fstream::out | std::fstream::app);
	
	std::string eString = "";
	
	for (int i = 0; i < password.length(); i++) {
		int ch = password[i];
		int binary[16];
		int j = 15;
		while (j >= 0) {
			if (ch > 0) {
				binary[j] = ch % 2;
				ch = ch / 2;
			}
			else {
				binary[j] = 0;
			}
			j--;
		}
		for (int j = 0; j < 16; j++) {
			eString.append(std::to_string(binary[j]));
		}

		passFile << eString.c_str() << std::endl;

		eString = "";
	}
	
	passFile.close();
	if (_chmod(tmpPassFile.c_str(), _S_IREAD) == -1) {
		std::cout << "Could not change the permission mode" << std::endl;
	}
}

//	Decrypt password
std::string User::decrypt() {
	std::string dString = "";
	std::string tmpString;
	while (passFile >> tmpString) {
		int decimal = 0;
		for (int i = 0; i < 16; i++) {
			decimal += (tmpString[i] == '0' ? 0 : 1) * std::pow(2, 15 - i);
		}
		dString += char(decimal);
	}
	return dString;
}

//	User Loop
std::string User::loop() {
	while (running) {
		render();
		input();
	}
	if (quit) {
		return "";
	}
	return username;
}

//	User Render
void User::render() {
	int textWidth, textHeight, charWidth, charHeight;

	SDL_RenderClear(ren);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = winWidth;
	rect.h = winHeight;
	SDL_RenderDrawRect(ren, &rect);

	//	Head Text
	TTF_SizeText(headFont, head, &textWidth, &textHeight);
	draw("GameLib", headFont, (winWidth / 2) - (textWidth / 2), 10, 255, 255, 0);


	TTF_SizeText(userFont, "A", &charWidth, &charHeight);
	if (!putPassword) {
		TTF_SizeText(normalFont, "Enter your username", &textWidth, &textHeight);
		draw("Enter your username", normalFont, (winWidth / 2) - (textWidth / 2), winHeight / 2 - (5 * textHeight) / 2, 255, 255, 255);
		
		//	Your username
		TTF_SizeText(userFont, username.c_str(), &textWidth, &textHeight);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_Rect userRect;
		userRect.x = (winWidth / 2) - (textWidth / 2) - 10;
		userRect.y = winHeight / 2 - charHeight / 2 - 5;
		userRect.w = username.length() * charWidth + 20;
		userRect.h = charHeight + 10;
		SDL_RenderDrawRect(ren, &userRect);

		std::string sTmp;
		for (int i = 0; i < username.length(); i++) {
			sTmp = username[i];
			draw(sTmp.c_str(), userFont, winWidth / 2 - textWidth / 2 + (i * charWidth), winHeight / 2 - textHeight / 2, 0, 0, 255);
		}
	}
	else {
		TTF_SizeText(normalFont, "Enter your password", &textWidth, &textHeight);
		draw("Enter your password", normalFont, (winWidth / 2) - (textWidth / 2), winHeight / 2 - (5 * textHeight) / 2, 255, 255, 255);
		
		//	Password
		TTF_SizeText(userFont, password.c_str(), &textWidth, &textHeight);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_Rect passRect;
		passRect.x = (winWidth / 2) - (textWidth / 2) - 10;
		passRect.y = winHeight / 2 - charHeight / 2 - 5;
		passRect.w = password.length() * charWidth + 20;
		passRect.h = charHeight + 10;
		SDL_RenderDrawRect(ren, &passRect);

		std::string sTmp;
		for (int i = 0; i < password.length(); i++) {
			if (showPass) {
				sTmp = password[i];
			}
			else {
				sTmp = "*";
			}
			draw(sTmp.c_str(), userFont, winWidth / 2 - textWidth / 2 + (i * charWidth), winHeight / 2 - textHeight / 2, 0, 0, 255);
		}
	}

	TTF_SizeText(instructionFont, "Press Enter to proceed", &textWidth, &textHeight);
	draw("Press Enter to proceed", instructionFont, winWidth / 2 - textWidth / 2, winHeight / 2 + (5 * textHeight / 2), 255, 255, 0);

	if (error) {
		TTF_SizeText(instructionFont, "Invalid! Enter correct details!", &textWidth, &textHeight);
		draw("Invalid! Enter correct details!", instructionFont, winWidth / 2 - textWidth / 2, winHeight / 2 + (7 * textHeight / 2), 255, 0, 0);
	}

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	
	SDL_RenderPresent(ren);
}

//	Draw Text
void User::draw(const char* msg, TTF_Font* font, int x, int y, int r, int g, int b) {
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

//	User Input
void User::input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = 0;
			quit = 1;
		}

		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				running = 0;
				quit = 1;
			}
			if (event.key.keysym.sym == SDLK_RETURN) {
				if (!putPassword) {
					if (username.length() > 0) {
						if (checkError() != 0) {
							std::string userFolder = "Resources/User";
							int status = _mkdir(userFolder.c_str());
							userFolder += "/" + username;
							status = _mkdir(userFolder.c_str());
							putPassword = true;
						}
						else {
							error = true;
						}
					}
					else {
						error = true;
					}
				}
				else {
					if (password.length() > 0) {
						std::string tmpPassFile = "Resources/User/" + username + "/pass.txt";
						const char* passFileName = tmpPassFile.c_str();
						passFile.open(passFileName, std::fstream::in);
						if (!passFile) {
							encrypt();
							running = 0;
						}
						else {
							std::string tmpPass = decrypt();
							if (tmpPass == password) {
								running = 0;
							}
							else {
								error = true;
							}
						}
					}
					else {
						error = true;
					}
				}
			}
			if (event.key.keysym.sym == SDLK_BACKSPACE || event.key.keysym.sym == SDLK_DELETE) {
				if (!putPassword) {
					if (username.length() > 0) {
						username.pop_back();
					}
				}
				else {
					if (password.length() > 0) {
						password.pop_back();
					}
				}
				error = false;
			}
		}

		if (event.type == SDL_TEXTINPUT) {
			std::string cTmp = event.text.text;
			if (!putPassword) {
				username += cTmp;
			}
			else {
				password += cTmp;
			}
		}
	}
}