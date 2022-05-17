#ifndef LIFEHEART_H
#define LIFEHEART_H

#include "Game.h"

class LifeHeart{
 public:
    LifeHeart(int x,int y);

    ~LifeHeart();

    void LoadFromFile(std::string path, SDL_Renderer* gRenderer);

    void Render(SDL_Rect* currentClip, SDL_Renderer *gRenderer, LTexture gHeartTexture);

    int GetPosX();

	int GetPosY();

	int GetWidth();

	int GetHeight();
 private: 
    int posX, posY;

	int eWidth, eHeight;

	SDL_Texture *HeartTexture;
};
#endif