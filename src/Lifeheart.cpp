#include"Lifeheart.h"

LifeHeart::LifeHeart(int x,int y){
    posX = x;
    posY = y;
}
LifeHeart::~LifeHeart(){
    posX = 0;
    posY = 0;

    eWidth = 0;
    eHeight = 0;

    if(HeartTexture != nullptr)
    {
        HeartTexture = nullptr;
    }
}
void LifeHeart::LoadFromFile(std::string path , SDL_Renderer* gRenderer)
{   
    SDL_Surface* Surface = IMG_Load(path.c_str());
    if(Surface == nullptr)
    {
        LogError("Can not load image", IMG_ERROR);
    }
    else{
        SDL_SetColorKey(Surface,SDL_TRUE,SDL_MapRGB(Surface->format, 0, 255, 255));
        
        HeartTexture = SDL_CreateTextureFromSurface(gRenderer,Surface);
        if(HeartTexture == nullptr)
        {
            LogError("Can not create texture from surface :",SDL_ERROR);
        }
        else{
            eWidth = Surface->w;
            eHeight = Surface->h;
        }
        SDL_FreeSurface(Surface);
    }
}
void LifeHeart::Render(SDL_Rect* currentClip, SDL_Renderer *gRenderer, LTexture gHeartTexture)
{
	gHeartTexture.Render(posX, posY, gRenderer, currentClip);
}