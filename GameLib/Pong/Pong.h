#pragma once
#include <iostream>
#include "Color.h"
#include <conio.h>
#include <time.h>

enum eDir {
	STOP = 0,
	LEFT = 1,
	UPLEFT = 2,
	DOWNLEFT = 3,
	RIGHT = 4,
	UPRIGHT = 5,
	DOWNRIGHT = 6
};

class Ball
{
private:
	int x, y;
	int origX, origY;
	eDir direction;
public:
	Ball(int posX, int posY) {
		origX = posX;
		origY = posY;
		x = posX;
		y = posY;
		direction = STOP;
	}
	void reset() {
		x = origX, y = origY;
		direction = STOP;
	}
	void changeDir(eDir d) {
		direction = d;
	}
	void randomDirection() {
		direction = (eDir)((rand() % 6) + 1);
	}
	inline int getX() {
		return x;
	}
	inline int getY() {
		return y;
	}
	inline eDir getDirection() {
		return direction;
	}
	void move();
	friend std::ostream& operator<<(std::ostream& o, Ball b) {
		o << "Ball [" << b.x << "," << b.y << "][" << b.direction << "] ";
		return o;
	}
};

void Ball::move() {
	switch (direction) {
	case STOP:
		break;
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	case UPLEFT:
		y--;
		x--;
		break;
	case DOWNLEFT:
		y++;
		x--;
		break;
	case UPRIGHT:
		y--;
		x++;
		break;
	case DOWNRIGHT:
		y++;
		x++;
		break;
	default:
		break;
	}
}



class Paddle
{
private:
	int x, y;
	int originalX, originalY;
public:
	Paddle() {
		x = y = 0;
	}
	Paddle(int posX, int posY) : Paddle()
	{
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
	}
	inline void reset() {
		x = originalX;
		y = originalY;
	}
	inline int getX() {
		return x;
	}
	inline int getY() {
		return y;
	}
	inline void moveUp() {
		y--;
	}
	inline void moveDown() {
		y++;
	}
	friend std::ostream& operator<<(std::ostream& o, Paddle b) {
		o << "Paddle [" << b.x << "," << b.y << "]";
		return o;
	}
};


class GameManager
{
private:
	int width, height;
	int score1, score2;
	char up1, down1, up2, down2;
	bool quit;
	Ball* ball;
	Paddle* player1;
	Paddle* player2;
public:
	GameManager(int w, int h)
	{
		srand(time(NULL));
		quit = false;
		up1 = 'w';
		down1 = 's';
		up2 = '8';
		down2 = '2';
		score1 = score2 = 0;
		width = w;
		height = h;
		ball = new Ball(w / 2, h / 2);
		player1 = new Paddle(1, h / 2 - 3);
		player2 = new Paddle(w - 3, h / 2 - 3);
	}
	~GameManager() {
		delete ball, player1, player2;
	}
	void ScoreUp(Paddle* player);
	void draw();
	void input();
	void logic();
	void run();
};

void GameManager::ScoreUp(Paddle* player) {
	if (player == player1) {
		score1++;
	}
	else if (player == player2) {
		score2++;
	}
	ball->reset();
	player1->reset();
	player2->reset();
}

void GameManager::draw() {
	system("cls");
	for (int i = 0; i < width + 2; i++) {
		std::cout << "\xB2";
	}
	std::cout << std::endl;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int ballX = ball->getX();
			int ballY = ball->getY();
			int player1X = player1->getX();
			int player2X = player2->getX();
			int player1Y = player1->getY();
			int player2Y = player2->getY();

			if (j == 0) {
				std::cout << Color(7) << "\xB2" << Color(7);
			}
			if (ballX == j && ballY == i) {
				std::cout << Color(9) << "O" << Color(7); // ball
			}
			else if (player1X == j && player1Y == i) {
				std::cout << Color(6) << "\xDB" << Color(7); // player1
			}
			else if (player2X == j && player2Y == i) {
				std::cout << Color(6) << "\xDB" << Color(7); // player2
			}
			else if (player1X == j && player1Y + 1 == i) {
				std::cout << Color(6) << "\xDB" << Color(7); // player1
			}
			else if (player1X == j && player1Y + 2 == i) {
				std::cout << Color(6) << "\xDB" << Color(7); // player1
			}
			else if (player1X == j && player1Y + 3 == i) {
				std::cout << Color(6) << "\xDB" << Color(7); // player1
			}
			else if (player2X == j && player2Y + 1 == i) {
				std::cout << Color(6) << "\xDB" << Color(7); // player1
			}
			else if (player2X == j && player2Y + 2 == i) {
				std::cout << Color(6) << "\xDB" << Color(7); // player1
			}
			else if (player2X == j && player2Y + 3 == i) {
				std::cout << Color(6) << "\xDB" << Color(7); // player1
			}
			else {
				std::cout << Color(7) << " ";
			}

			if (j == width - 1) {
				std::cout << Color(7) << "\xB2" << Color(7);
			}
		}
		std::cout << std::endl;
	}

	for (int i = 0; i < width + 2; i++) {
		std::cout << Color(7) << "\xB2" << Color(7);
	}
	std::cout << std::endl;
}

void GameManager::input() {
	ball->move();

	int ballX = ball->getX();
	int ballY = ball->getY();
	int player1X = player1->getX();
	int player2X = player2->getX();
	int player1Y = player1->getY();
	int player2Y = player2->getY();

	if (_kbhit()) {
		char ch = _getch();
		if (ch == up1) {
			if (player1Y > 0) {
				player1->moveUp();
			}
		}
		if (ch == up2) {
			if (player2Y > 0) {
				player2->moveUp();
			}
		}
		if (ch == down1) {
			if (player1Y < height -	4) {
				player1->moveDown();
			}
		}
		if (ch == down2) {
			if (player2Y < height - 4) {
				player2->moveDown();
			}
		}

		if (ball->getDirection() == STOP) {
			ball->randomDirection();
		}

		if (ch == 'q') {
			quit = true;
		}
	}
}

void GameManager::logic() {
	int ballX = ball->getX();
	int ballY = ball->getY();
	int player1X = player1->getX();
	int player2X = player2->getX();
	int player1Y = player1->getY();
	int player2Y = player2->getY();

	//left paddle
	for (int i = 0; i < 4; i++) {
		if (ballX == player1X + 1) {
			if (ballY == player1Y + i) {
				ball->changeDir((eDir)((rand() % 3) + 4));
			}
		}
	}

	//right paddle
	for (int i = 0; i < 4; i++) {
		if (ballX == player2X - 1) {
			if (ballY == player2Y + i) {
				ball->changeDir((eDir)((rand() % 3) + 1));
			}
		}
	}

	//bottom wall
	if (ballY == height - 1) {
		ball->changeDir(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT );
	}
	//top wall
	if (ballY == 0) {
		ball->changeDir(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
	}

	//right wall 
	if (ballX == width - 1) {
		ScoreUp(player1);
	}
	//left wall 
	if (ballX == 0) {
		ScoreUp(player2);
	}
}

void GameManager::run() {
	while (!quit) {
		draw();
		input();
		logic();
		Sleep(50);
	}
}