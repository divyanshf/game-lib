#include "Object.h"

void Object::setDest(int x, int y, int w, int h) {
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;
}
void Object::setDest(int x, int y) {
    dest.x = x;
    dest.y = y;
}
void Object::setSource(int x, int y, int w, int h) {
    src.x = x;
    src.y = y;
    src.w = w;
    src.h = h;
}
void Object::setImage(std::string filename, SDL_Renderer* ren) {
    SDL_Surface* surf;
    if (IMG_Load(filename.c_str()) < 0) {
        std::cout << "Failed at IMG_Load()" << IMG_GetError() << std::endl;
    }
    else {
        surf = IMG_Load(filename.c_str());
    }

    //tex = SDL_CreateTextureFromSurface(ren, surf);
    tex = IMG_LoadTexture(ren, filename.c_str());

}
