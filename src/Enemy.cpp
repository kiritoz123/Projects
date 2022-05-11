#include "Enemy.h"

Enemy::Enemy(int _type){
    posX = 0;
    posY = 0;

    eWidth = 0;
    eHeight = 0;

    type = _type;

    if(type == IN_AIR_ENEMY)
    {
        posX = rand() % (SCREEN_WIDTH + ENEMY_POSITION_RANGE) + SCREEN_WIDTH;
		posY = rand() % 420 + 415;
    }
    else if(type == ON_GROUND_ENEMY)
    {
        posX = rand() % (SCREEN_WIDTH + ENEMY_POSITION_RANGE) + 100;
        posY = GROUND - 8;
    }
    EnemyTexture = nullptr;
}

Enemy::~Enemy()
{
    posX = 0;
    posY = 0;

    eWidth = 0;
    eHeight = 0;

    type = 0;
    if(EnemyTexture != nullptr)
    {
        EnemyTexture = nullptr;
    }
}

void Enemy::LoadFromFile(std::string path , SDL_Renderer* gRenderer)
{   
    SDL_Surface* Surface = IMG_Load(path.c_str());
    if(Surface == nullptr)
    {
        LogError("Can not load image", IMG_ERROR);
    }
    else{
        SDL_SetColorKey(Surface,SDL_TRUE,SDL_MapRGB(Surface->format, 0, 255, 255));
        
        EnemyTexture = SDL_CreateTextureFromSurface(gRenderer,Surface);
        if(EnemyTexture == nullptr)
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

void Enemy::Move(const int &acceleration)
{
	posX += -(ENEMY_SPEED + acceleration);
	if (posX + MAX_ENEMY_WIDTH < 0)
	{
		posX = rand() % (SCREEN_WIDTH + ENEMY_POSITION_RANGE) + SCREEN_WIDTH;

		if (type == IN_AIR_ENEMY)
		{
			posY = rand() % 420 + 415 ;
		}
	}
}

void Enemy::Render(SDL_Renderer* gRenderer, SDL_Rect* currentClip)
{
	SDL_Rect renderSpace = { posX, posY, eWidth, eHeight };
	if (currentClip != nullptr)
	{
		renderSpace.w = currentClip->w;
		renderSpace.h = currentClip->h;
	}
	SDL_RenderCopy(gRenderer, EnemyTexture, currentClip, &renderSpace);
}

int Enemy::GetType()
{
	if (type == IN_AIR_ENEMY)
	{
		return IN_AIR_ENEMY;
	}
	else
	{
		return ON_GROUND_ENEMY;
	}
}

int Enemy::GetSpeed(const int &acceleration)
{
	return ENEMY_SPEED + acceleration;
}

int Enemy::GetPosX()
{
	return posX;
}

int Enemy::GetPosY()
{
	return posY;
}

int Enemy::GetWidth()
{
	return eWidth;
}

int Enemy::GetHeight()
{
	return eHeight;
}
