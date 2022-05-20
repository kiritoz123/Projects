#include "Game_Base.h"
#include "Game.h"
#include "LTexture.h"
#include "Button.h"
#include "Character.h"
#include "Enemy.h"
#include "Lifeheart.h"

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Color textColor = { 0, 0, 0 };
TTF_Font* gFont = nullptr;
Mix_Music* gMusic = nullptr;
Mix_Music* gMenuMusic = nullptr;
Mix_Chunk* gClick = nullptr;
Mix_Chunk* gJump = nullptr;
Mix_Chunk* gLose = nullptr;

SDL_Rect gPlayButton[BUTTON_TOTAL];
SDL_Rect gHighScoreButton[BUTTON_TOTAL];
SDL_Rect gExitButton[BUTTON_TOTAL];
SDL_Rect gBackButton[BUTTON_TOTAL];
SDL_Rect gPauseButton[BUTTON_TOTAL];
SDL_Rect gContinueButton[BUTTON_TOTAL];
SDL_Rect gPlayAgainButton[BUTTON_TOTAL];
SDL_Rect gCharacterClips[RUNNING_FRAMES];
SDL_Rect gEnemyClips[FLYING_FRAMES];

LTexture gName;
LTexture gMenuName;
LTexture gMenuTexture;
LTexture gHeart1;
LTexture gHeart2;
LTexture gHeart3;
LTexture gBackgroundTexture;
LTexture gCharacterTexture;
LTexture gGroundTexture;
LTexture gPlayButtonTexture;
LTexture gHighScoreButtonTexture;
LTexture gExitButtonTexture;
LTexture gBackButtonTexture;
LTexture gPauseButtonTexture;
LTexture gContinueButtonTexture;
LTexture gLoseTexture;
LTexture gText1Texture;
LTexture gScoreTexture;
LTexture gText2Texture;
LTexture gHighScoreTexture;

Button PlayButton(PLAY_BUTON_POSX, PLAY_BUTTON_POSY);
Button HighScore(HELP_BUTTON_POSX, HELP_BUTTON_POSY);
Button ExitButton(EXIT_BUTTON_POSX, EXIT_BUTTON_POSY);
Button BackButton(BACK_BUTTON_POSX, BACK_BUTTON_POSY);
Button PauseButton(PAUSE_BUTTON_POSX, PAUSE_BUTTON_POSY);
Button ContinueButton(CONTINUE_BUTTON_POSX, CONTINUE_BUTTON_POSY);

LifeHeart Heart1(80,20);
LifeHeart Heart2(140,20);
LifeHeart Heart3(200,20);
Character character;

int main(int argc, char* argv[])
{
	if (!Init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!LoadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{	
			bool Quit_Menu = false;
			bool Play_Again = false;
			std::string playname ;
			Mix_PlayMusic(gMenuMusic, IS_REPEATITIVE);
		do{
			while (!Quit_Menu)
			{	
				SDL_Event e_mouse;
				while (SDL_PollEvent(&e_mouse) != 0)
				{	bool Quit_Game = false;
					if (e_mouse.type == SDL_QUIT)
					{	Quit_Game = true;
						Quit_Menu = true;
					}
					
					
					HandlePlayButton(&e_mouse, gPlayButton, PlayButton, BackButton,gName,gMenuName,
					gBackButtonTexture,gRenderer,Play_Again,Quit_Menu,gClick,playname,gFont);
						
					HandleHighScoreButton(&e_mouse, gBackButton,
									 HighScore, BackButton, 
									 gScoreTexture, gBackButtonTexture,
									 gRenderer, Quit_Game, gClick,gFont);

					HandleExitButton(&e_mouse, ExitButton, Quit_Game, gClick);
					if (Quit_Game == true)
					{
						return 0;
					}
				}
				
				gMenuTexture.Render(0, 0, gRenderer);

				SDL_Rect* currentClip_Play = &gPlayButton[PlayButton.currentSprite];
				PlayButton.Render(currentClip_Play, gRenderer, gPlayButtonTexture);

				SDL_Rect* currentClip_HighScore = &gHighScoreButton[HighScore.currentSprite];
				HighScore.Render(currentClip_HighScore, gRenderer, gHighScoreButtonTexture);

				SDL_Rect* currentClip_Exit = &gExitButton[ExitButton.currentSprite];
				ExitButton.Render(currentClip_Exit, gRenderer, gExitButtonTexture);
				
				SDL_RenderPresent(gRenderer);
			} 
			
				Quit_Menu = false;
				srand(time(NULL));
				int check = 0;
				int count = 3;
				int time = 0;
				int score = 0;
				int acceleration = 0;
				int frame_Character = 0;
				int frame_Enemy = 0;
				std::string highscore = GetHighScoreFromFile("src/high_score.txt");
				int hscore = GetHighScore();
				SDL_Event e;
				Enemy enemy1(ON_GROUND_ENEMY);
				Enemy enemy2(ON_GROUND_ENEMY);
				Enemy enemy3(IN_AIR_ENEMY);
				
				Mix_PlayMusic(gMusic, IS_REPEATITIVE);
				GenerateEnemy(enemy1, enemy2, enemy3, gEnemyClips, gRenderer);

				int OffsetSpeed_Ground = BASE_OFFSET_SPEED;
				double OffsetSpeed_Background = BASE_OFFSET_SPEED;

				bool Quit = false;
				bool Game_State = true;
				while (!Quit)
				{
					if (Game_State)
					{
						UpdateGameTimeAndScore(time, acceleration, score);

						while (SDL_PollEvent(&e) != 0)
						{
							if (e.type == SDL_QUIT)
							{
								Quit = true;
								Play_Again = false;
							}

							HandlePauseButton(&e, gRenderer, gContinueButton,
								PauseButton, ContinueButton,
								gContinueButtonTexture, Game_State, gClick);

							character.HandleEvent(e, gJump);
						}
						SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
						SDL_RenderClear(gRenderer);

						RenderScrollingBackground( OffsetSpeed_Background,gBackgroundTexture, gRenderer);
						RenderScrollingGround(OffsetSpeed_Ground, acceleration, gGroundTexture, gRenderer);


						character.Move();
						SDL_Rect* currentClip_Character = nullptr;
						if (character.onGround())
						{
							currentClip_Character = &gCharacterClips[frame_Character / FRAME_CHAR];
							character.Render(currentClip_Character, gRenderer, gCharacterTexture);
						}
						else
						{
							currentClip_Character = &gCharacterClips[0];
							character.Render(currentClip_Character, gRenderer, gCharacterTexture);
						}

						
						enemy1.Move(acceleration);
						enemy1.Render(gRenderer);
			
						enemy2.Move(acceleration);
						enemy2.Render(gRenderer);

						SDL_Rect* currentClip_Enemy = &gEnemyClips[frame_Enemy / FRAME_ENEMY];
						enemy3.Move(acceleration);
						enemy3.Render(gRenderer, currentClip_Enemy);


						SDL_Rect* currentClip_Pause = &gPauseButton[PauseButton.currentSprite];
						PauseButton.Render(currentClip_Pause, gRenderer, gPauseButtonTexture);


						DrawPlayerScore(gText1Texture, gScoreTexture, textColor, gRenderer, gFont, score);
						//DrawPlayerHighScore(gText2Texture, gHighScoreTexture, textColor, gRenderer, gFont, highscore);
						
						SDL_Rect* currentClip_heart = nullptr;
						if(count == 3){
						Heart1.Render(currentClip_heart,gRenderer,gHeart1);
						Heart2.Render(currentClip_heart,gRenderer,gHeart2);
						Heart3.Render(currentClip_heart,gRenderer,gHeart3);}
						if(count == 2){
							Heart1.Render(currentClip_heart,gRenderer,gHeart1);
							Heart2.Render(currentClip_heart,gRenderer,gHeart2);
						}
						if(count == 1){
							Heart1.Render(currentClip_heart,gRenderer,gHeart1);
						}
						if (CheckEnemyColission(character,
							enemy1, enemy2, enemy3,
							currentClip_Character, currentClip_Enemy,check))
						{
							if(check != 0) {
								count--;
								Mix_PlayChannel(MIX_CHANNEL, gLose, NOT_REPEATITIVE);
							}
							if(count == 0){
								Mix_PauseMusic();
								Mix_PlayChannel(MIX_CHANNEL, gLose, NOT_REPEATITIVE);
								UpdateHighScore(playname, score, highscore);
								//UpdateHighScore2(playname,score,hscore,0);
								Quit = true;
							}
						}
						if(time % 120 == 0) check = 0;

						//SDL_Delay(5);
						SDL_RenderPresent(gRenderer);
						CharFrame(frame_Character);
						EnemyFrame(frame_Enemy);
					}
					
				}

				DrawEndGameSelection(gLoseTexture, &e, gRenderer, Play_Again);
			
				if (!Play_Again)
				{
					enemy1.~Enemy();
					enemy2.~Enemy();
					enemy3.~Enemy();
				}
			}while(Play_Again);
		}
	}
	Close();

	return 0;
}


bool Init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_VIDEO) < 0)
	{
		LogError("Can not initialize SDL.", SDL_ERROR);
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			std::cout << "Warning: Linear texture filtering not enabled!";
		}

		gWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			LogError("Can not create window", SDL_ERROR);
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				LogError("Can not create renderer", SDL_ERROR);
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					LogError("Can not initialize SDL_image", IMG_ERROR);
					success = false;
				}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}

				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool LoadMedia()
{
	bool success = true;

	gMusic = Mix_LoadMUS("sound/bkgr_audio.wav");
	if (gMusic == nullptr)
	{
		LogError("Failed to load background music", MIX_ERROR);
		success = false;
	}

	gMenuMusic = Mix_LoadMUS("sound/menu_audio.wav");
	if (gMenuMusic == nullptr)
	{
		LogError("Failed to load menu music", MIX_ERROR);
		success = false;
	}

	gClick = Mix_LoadWAV("sound/mouse_click.wav");
	if (gClick == nullptr)
	{
		LogError("Failed to load mouse click sound", MIX_ERROR);
		success = false;
	}

	gJump = Mix_LoadWAV("sound/jump_sound.wav");
	if (gJump == nullptr)
	{
		LogError("Failed to load jumping sound", MIX_ERROR);
		success = false;
	}

	gLose = Mix_LoadWAV("sound/lose_sound.wav");
	if (gLose == nullptr)
	{
		LogError("Failed to load lose sound", MIX_ERROR);
		success = false;
	}

	else
	{
		gFont = TTF_OpenFont("font/pixel_font.ttf", 30);
		if (gFont == NULL)
		{
			LogError("Failed to load font", MIX_ERROR);
			success = false;
		}
		else
		{
			if (!gText1Texture.LoadFromRenderedText("Your score: ", gFont, textColor, gRenderer))
			{
				std::cout << "Failed to render text1 texture" << std::endl;
				success = false;
			}

			if (!gText2Texture.LoadFromRenderedText("High score: ", gFont, textColor, gRenderer))
			{
				std::cout << "Failed to render text2 texture" << std::endl;
				success = false;
			}
			
			if (!gMenuTexture.LoadFromFile("imgs/background/menu.png", gRenderer))
			{
				std::cout << "Failed to load menu image" << std::endl;
				success = false;
			}
			if(!gHeart1.LoadFromFile("imgs/background/heart.png",gRenderer)){
				std::cout<<"Failed to load heart image" << std::endl;
				success = false;
			}
			if(!gHeart2.LoadFromFile("imgs/background/heart.png",gRenderer)){
				std::cout<<"Failed to load heart image" << std::endl;
				success = false;
			}
			if(!gHeart3.LoadFromFile("imgs/background/heart.png",gRenderer)){
				std::cout<<"Failed to load heart image" << std::endl;
				success = false;
			}
			if(!gMenuName.LoadFromFile("imgs/background/menuName.png", gRenderer))
			{	std::cout << "Failed to load menuName image" << std::endl;
				success = false;
			}
			if (!gScoreTexture.LoadFromFile("imgs/background/highscore.png", gRenderer))
			{
				std::cout << "Failed to load highscore image" << std::endl;
				success = false;
			}

			if (!gPlayButtonTexture.LoadFromFile("imgs/button/big_button/play_button.png", gRenderer))
			{
				std::cout << "Failed to load play_button image" << std::endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < BUTTON_TOTAL; ++i)
				{
					gPlayButton[i].x = 150 * i;
					gPlayButton[i].y = 0;
					gPlayButton[i].w = 150;
					gPlayButton[i].h = 98;
				}
			}

			if (!gHighScoreButtonTexture.LoadFromFile("imgs/button/big_button/highscore_button.png", gRenderer))
			{
				std::cout << "Failed to load highscore_button image" << std::endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < BUTTON_TOTAL; ++i)
				{
					gHighScoreButton[i].x = 150 * i;
					gHighScoreButton[i].y = 0;
					gHighScoreButton[i].w = 150;
					gHighScoreButton[i].h = 98;
				}
			}

			if (!gBackButtonTexture.LoadFromFile("imgs/button/big_button/back_button.png", gRenderer))
			{
				std::cout << "Failed to load back_button image" << std::endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < BUTTON_TOTAL; ++i)
				{
					gBackButton[i].x = 100 * i;
					gBackButton[i].y = 0;
					gBackButton[i].w = 100;
					gBackButton[i].h = 78;
				}
			}

			if (!gExitButtonTexture.LoadFromFile("imgs/button/big_button/exit_button.png", gRenderer))
			{
				std::cout << "Failed to load exit_button image" << std::endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < BUTTON_TOTAL; ++i)
				{
					gExitButton[i].x = 150 * i;
					gExitButton[i].y = 0;
					gExitButton[i].w = 150;
					gExitButton[i].h = 98;
				}
			}

			if (!gPauseButtonTexture.LoadFromFile("imgs/button/big_button/pause_button.png", gRenderer))
			{
				std::cout << "Failed to load pause_button image " << std::endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < BUTTON_TOTAL; ++i)
				{
					gPauseButton[i].x = 22 * i;
					gPauseButton[i].y = 0;
					gPauseButton[i].w = 22;
					gPauseButton[i].h = 34;
				}
			}

			if (!gContinueButtonTexture.LoadFromFile("imgs/button/big_button/continue_button.png", gRenderer))
			{
				std::cout << "Failed to load continue_button image " << std::endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < BUTTON_TOTAL; ++i)
				{
					gContinueButton[i].x = 22 * i;
					gContinueButton[i].y = 0;
					gContinueButton[i].w = 22;
					gContinueButton[i].h = 34;
				}
			}

			for (int i = 0; i < BACKGROUND_LAYER; ++i)
			{
				if (!gBackgroundTexture.LoadFromFile(LAYER[i].c_str(), gRenderer))
				{
					std::cout << "Failed to load background image" << std::endl;
					success = false;
				}
			}

			if (!gGroundTexture.LoadFromFile("imgs/background/ground.png", gRenderer))
			{
				std::cout << "Failed to load ground image" << std::endl;
				success = false;
			}

			if (!gCharacterTexture.LoadFromFile("imgs/character/char.png", gRenderer))
			{
				std::cout << "Failed to load character_run image." << std::endl;
				success = false;
			}
			else
			{
				gCharacterClips[0].x = 57 * 0;
				gCharacterClips[0].y = 0;
				gCharacterClips[0].w = 57;
				gCharacterClips[0].h = 57;

				gCharacterClips[1].x = 57 * 1;
				gCharacterClips[1].y = 0;
				gCharacterClips[1].w = 57;
				gCharacterClips[1].h = 57;

				gCharacterClips[2].x = 57 * 2;
				gCharacterClips[2].y = 0;
				gCharacterClips[2].w = 57;
				gCharacterClips[2].h = 57;

				gCharacterClips[3].x = 57 * 3;
				gCharacterClips[3].y = 0;
				gCharacterClips[3].w = 57;
				gCharacterClips[3].h = 57;

				gCharacterClips[4].x = 57 * 4;
				gCharacterClips[4].y = 0;
				gCharacterClips[4].w = 57;
				gCharacterClips[4].h = 57;

				gCharacterClips[5].x = 57 * 5;
				gCharacterClips[5].y = 0;
				gCharacterClips[5].w = 57;
				gCharacterClips[5].h = 57;
			}

			if (!gLoseTexture.LoadFromFile("imgs/background/lose.png", gRenderer))
			{
				std::cout << "Failed to load lose image." << std::endl;
				success = false;
			}
		}
	}
	return success;
}

void Close()
{
	gMenuTexture.Free();
	gScoreTexture.Free();
	gCharacterTexture.Free();
	gGroundTexture.Free();
	gPlayButtonTexture.Free();
	gHighScoreButtonTexture.Free();
	gExitButtonTexture.Free();
	gBackButtonTexture.Free();
	gPauseButtonTexture.Free();
	gContinueButtonTexture.Free();
	gLoseTexture.Free();
	gText1Texture.Free();
	gScoreTexture.Free();
	gText2Texture.Free();
	gHighScoreTexture.Free();
	gHeart1.Free();
	gHeart2.Free();
	gHeart3.Free();
	for (int i = 0; i < BACKGROUND_LAYER; ++i)
	{
		gBackgroundTexture.Free();
	}

	Mix_FreeMusic(gMusic);
	Mix_FreeMusic(gMenuMusic);
	Mix_FreeChunk(gClick);
	Mix_FreeChunk(gLose);
	Mix_FreeChunk(gJump);
	gMusic = nullptr;
	gMenuMusic = nullptr;
	gClick = nullptr;
	gLose = nullptr;
	gJump = nullptr;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;

	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}
