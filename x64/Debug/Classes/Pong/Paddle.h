#ifndef PADDLE_H
#define PADDLE_H

#include "../Object/Object.h"

class Paddle : public Object
{
private:
	int x, y;
	int originalX, originalY;
public:
	Paddle() {
		x = y = 0;
	}
	Paddle(int posX, int posY) : Paddle() {
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
	}
	inline void reset() {
		x = originalX;
		y = originalY;
	}
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline void moveUp() {
		y -= 13;
	}
	inline void moveDown() {
		y += 13;
	}
};

#endif // !PADDLE_H
