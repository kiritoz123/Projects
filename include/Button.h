#ifndef BUTTON_H_
#define BUTTON_H_

#define SMALL_BUTTON 1
#define COMMON_BUTTON 2

#include "Game_Base.h"
#include "LTexture.h"

const int COMMON_BUTTON_WIDTH = 150;
const int COMMON_BUTTON_HEIGHT = 98;
const int SMALL_BUTTON_WIDTH = 22;
const int SMALL_BUTTON_HEIGHT = 34;



class Button
{
    public:
        ButtonSprite currentSprite;

        Button();

        Button(int x, int y);

        void SetPosition(int x, int y);

        bool IsInside(SDL_Event *e,int size);

        void Render(SDL_Rect* currentClip,SDL_Renderer* gRenderer,LTexture gButtonTexture);
    private:
        SDL_Point position;
};
#endif