#ifndef BALL_H
#define BALL_H

#include <cstdlib>
#include "Object.h"

enum class eDir {
	STOP = 0,
	LEFT = 1,
	RIGHT = 2,
	UPLEFT = 3,
	UPRIGHT = 4,
	DOWNLEFT = 5,
	DOWNRIGHT = 6
};

class Ball : public Object
{
private:
	int x, y;
	int originalX, originalY;
	int speed;
	eDir direction;
public:
	Ball(int posX, int posY) {
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
		speed = 1;
		direction = eDir::STOP;
	}
	void reset() {
		x = originalX;
		y = originalY;
		direction = eDir::STOP;
	}
	void changeDirection(eDir d) {
		direction = d;
	}
	void randomDirection() {
		direction = (eDir)((rand() % 6) + 1);
	}
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline eDir getDirection() { return direction; }
	void move() {
		switch (direction) {
		case eDir::STOP:
			break;
		case eDir::LEFT:
			x -= speed;
			break;
		case eDir::RIGHT:
			x += speed;
			break;
		case eDir::UPLEFT:
			x -= speed;
			y -= speed;
			break;
		case eDir::UPRIGHT:
			x += speed;
			y -= speed;
			break;
		case eDir::DOWNLEFT:
			x -= speed;
			y += speed;
			break;
		case eDir::DOWNRIGHT:
			x += speed;
			y += speed;
			break;
		default:
			break;
		}
	}
};

#endif // !BALL_H
