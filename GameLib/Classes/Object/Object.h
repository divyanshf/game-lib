#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

class Object {
private:
	SDL_Rect dest;
	SDL_Rect src;
	SDL_Texture* tex;
	bool solid;
public:
	Object() { solid = 1; }
	SDL_Rect getDest() const { return dest; }
	SDL_Rect getSource() const { return src; }
	void setDest(int x, int y, int w, int h);
	void setDest(int x, int y);
	void setSource(int x, int y, int w, int h);
	void setImage(std::string filename, SDL_Renderer* ren);
	SDL_Texture* getTex() const { return tex; }
	void setSolid(bool s) { solid = s; }
	bool getSolid()const { return solid; }
	int getDX() const { return dest.x; }
	int getDY() const { return dest.y; }
	int getDW() const { return dest.w; }
	int getDH() const { return dest.h; }
};


#endif //OBJECT_H
