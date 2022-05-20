#include "LTexture.h"
LTexture::LTexture()
{
    mTexture = nullptr;

    mWidth = 0;
    mHeight = 0;
}
LTexture::~LTexture(){
    Free();
}

void LTexture::Free(){
    if(mTexture != nullptr)
    {
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}
bool LTexture::LoadFromRenderedText(std::string textureText,TTF_Font *gFont
, SDL_Color textColor, SDL_Renderer *gRenderer)
{
    Free();
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont,textureText.c_str(),textColor);
    if(textSurface ==NULL)
    {
        std::cout << "Unable to render text surface! SDL_ttf Error:" << TTF_GetError()<< std::endl ;
    }
    else
    {
        mTexture = SDL_CreateTextureFromSurface(gRenderer,textSurface);
        if(mTexture == NULL)
        {
            std::cout << "Unable to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;

        }
        else{
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }
    return mTexture ;
}
bool LTexture::LoadFromFile(std::string path,SDL_Renderer *gRenderer)
{
    Free();
    SDL_Surface* Surface = IMG_Load(path.c_str());
    if(Surface == nullptr)
    {
        LogError("Can not load image.",IMG_ERROR);
    }
    else
    {
        SDL_SetColorKey(Surface,SDL_TRUE,SDL_MapRGB(Surface->format,0,255,255));

        mTexture = SDL_CreateTextureFromSurface(gRenderer,Surface);
        if(mTexture == nullptr)
        {
            LogError("can not create texture from surface.",SDL_ERROR);
        }
        else{
            mWidth = Surface->w;
            mHeight = Surface->h;
        }
        SDL_FreeSurface(Surface);
    }
    return mTexture;
}
void LTexture::Render(SDL_Renderer* des, SDL_Rect* clip )
{
    SDL_Rect renderQuad = {bRect.x, bRect.y, bRect.w, bRect.h} ;
    SDL_RenderCopy(des, mTexture, clip, &renderQuad) ;
}
void LTexture::Render(int x,int y,SDL_Renderer* gRenderer,SDL_Rect* clip)
{
    SDL_Rect renderer = {x,y,mWidth,mHeight};
    if(clip != nullptr)
    {
        renderer.w = clip->w;
        renderer.h = clip->h;
    }
    SDL_RenderCopy(gRenderer,mTexture,clip,&renderer);
}
int LTexture::GetWidth(){
    return mWidth;
}

int LTexture::GetHeight()
{
    return mHeight;
}