#ifndef GAME_H_
#define GAME_H_

#include "Game_Base.h"
#include "Button.h"
#include "Enemy.h"
#include "Character.h"

bool Init();
bool LoadMedia();
void Close();

int GetHighScore();
std::string GetHighScoreFromFile(std::string path);
void UpdateHighScore2(std::string& Name, int &score, int& oldHighScore, int type);
void UpdateHighScore(std::string path,
	const int& score, 
	const std::string& old_high_score);
void UpdateGameTimeAndScore(int& time, int& speed, int& score);

void RenderScrollingBackground(double& offsetSpeed,
	LTexture gBackgroundTexture,
	SDL_Renderer* gRenderer);

void RenderScrollingGround(int& speed,
    const int acceleration,
    LTexture gGroundTexture,
    SDL_Renderer* gRenderer);

void HandlePlayButton(SDL_Event* e,
	SDL_Rect(&gBackButton)[BUTTON_TOTAL],
	Button& PlayButton, 
	Button& BackButton,
	LTexture gName,LTexture gMenuName,
	LTexture gBackButtonTexture, 
	SDL_Renderer *gRenderer, 
    bool &play,
	bool &Quit_game, 
	Mix_Chunk *gClick,std::string& playname,TTF_Font* gFont);

void HandleHighScoreButton(SDL_Event* e,
	SDL_Rect(&gBackButton)[BUTTON_TOTAL],
	Button& HighScoreButton, 
	Button& BackButton, 
	LTexture gHighScoreTexture,
	LTexture gBackButtonTexture, 
	SDL_Renderer *gRenderer, 
	bool &Quit_game, 
	Mix_Chunk *gClick,TTF_Font* gFont);


void HandleExitButton(SDL_Event* e,
    Button& ExitButton,
    bool& Quit,
    Mix_Chunk* gClick);

void HandleContinueButton(Button ContinueButton,
    LTexture gContinueButtonTexture,
    SDL_Event* e,
    SDL_Renderer* gRenderer,
    SDL_Rect(&gContinueButton)[BUTTON_TOTAL],
    bool& Game_State, Mix_Chunk* gClick);

void HandlePauseButton(SDL_Event* e,
    SDL_Renderer* gRenderer,
    SDL_Rect(&gContinueButton)[BUTTON_TOTAL],
    Button& PauseButton,
    Button ContinueButton,
    LTexture gContinueButtonTexture,
    bool& game_state,
    Mix_Chunk* gClick);

void GenerateEnemy(Enemy& enemy1,
    Enemy& enemy2,
    Enemy& enemy3,
    SDL_Rect(&gEnemyClips)[FLYING_FRAMES],
    SDL_Renderer* gRenderer);

bool CheckColission(Character character,
    SDL_Rect* char_clip,
    Enemy enemy,
    SDL_Rect* enemy_clip = nullptr);

bool CheckEnemyColission(Character character,
	Enemy enemy1,
	Enemy enemy2, 
	Enemy enemy3,
	SDL_Rect* char_clip,
	SDL_Rect* enemy_clip,int& check
	);

void CharFrame(int& frame);

void EnemyFrame(int& frame);

void DrawPlayerScore(LTexture gTextTexture,
    LTexture gScoreTexture,
    SDL_Color textColor,
    SDL_Renderer* gRenderer,
    TTF_Font* gFont,
    const int& score);

void DrawPlayerHighScore(LTexture gTextTexture,
    LTexture gHighScoreTexture,
    SDL_Color textColor,
    SDL_Renderer* gRenderer,
    TTF_Font* gFont, 
    const std::string& HighScore);

void DrawEndGameSelection(LTexture gLoseTexture,
    SDL_Event* e,
    SDL_Renderer* gRenderer,
    bool& Play_Again);
std::string InputText(SDL_Event e,bool& quit_menu,
        LTexture& gName, LTexture& gMenuName,
        TTF_Font* gNameFont, SDL_Renderer* screen,
        Mix_Chunk *gClickMusic);
void RenderHighScore(SDL_Renderer* screen, TTF_Font* gFont);
#endif 