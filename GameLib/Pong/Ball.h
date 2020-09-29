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
	eDir direction;
public:
	Ball(int posX, int posY) {
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
		direction = eDir::STOP;
	}
	void reset() {
		x = originalX;
		y = originalY;
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
			x--;
			break;
		case eDir::RIGHT:
			x++;
			break;
		case eDir::UPLEFT:
			x--;
			y--;
			break;
		case eDir::UPRIGHT:
			x++;
			y--;
			break;
		case eDir::DOWNLEFT:
			x--;
			y++;
			break;
		case eDir::DOWNRIGHT:
			x++;
			y++;
			break;
		default:
			break;
		}
	}
};

#endif // !BALL_H
