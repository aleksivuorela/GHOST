#include "MainGame.h"
#include "Tile.h"
#include <stdio.h>
#include <iostream>
#include <SDL_image.h>

//Player
Player player;

//Level
Level level;

//Renderer
SDL_Renderer* renderer = NULL;

//Tile texture
Texture tileTexture;

//Enemy texture
Texture enemyTexture;

//Bullet texture
Texture bulletTexture;

//Bullets vector
std::vector<Bullet> bulletVec;

//Enemies vector
std::vector<Enemy> enemyVec;

//State of the game
GameState gameState;

//Victory = 1, defeat = 0
bool victory;

MainGame::MainGame()
{
	//Initialize
	_window = NULL;
	_camera.x = 0;
	_camera.y = 0;
	_camera.w = SCREEN_WIDTH;
	_camera.h = SCREEN_HEIGHT;
	gameState = GameState::PLAY;
	victory = 0;
}

void MainGame::run()
{
	//Start up SDL and create window
	if (!init())
	{
		close();
		printf("Failed to initialize!\n");
		std::cout << "Press any key to continue...";
		int tmp;
		std::cin >> tmp;
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			close();
			printf("Failed to load media!\n");
			std::cout << "Press any key to continue...";
			int tmp;
			std::cin >> tmp;
		}
		else
		{
			//Enter the game loop
			gameLoop();
		}
	}
}

bool MainGame::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		_window = SDL_CreateWindow("GHOST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (_window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool MainGame::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load player texture
	if (!player.loadTexture())
	{
		success = false;
	}

	//Load tile texture
	if (!tileTexture.loadFromFile("textures/tiles.png"))
	{
		printf("Failed to load tile texture!\n");
		success = false;
	}

	//Load enemy texture
	if (!enemyTexture.loadFromFile("textures/enemy.bmp"))
	{
		printf("Failed to load enemy texture!\n");
		success = false;
	}

	//Load bullet texture
	if (!bulletTexture.loadFromFile("textures/bullet.bmp"))
	{
		printf("Failed to load bullet texture!\n");
		success = false;
	}

	//Load tile map
	if (!level.setTiles())
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	return success;
}

void MainGame::close()
{
	//Free loaded images
	player.free();
	tileTexture.free();
	enemyTexture.free();
	bulletTexture.free();

	//Destroy window    
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(_window);
	_window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void MainGame::gameLoop()
{
	//Will loop until gameState is no longer set to PLAY
	while (gameState == GameState::PLAY)
	{
		//Process input
		processInput(_camera);
		//Play the game
		playGame();
	}

	//Game is over
	if (gameState == GameState::OVER)
	{
		if (gameOver())
		{
			//User wants to restart the game
			restart();
		}
		else
		{ 
			//Free resources and close SDL
			close();
		}
	}

	//User wants to quit 
	if (gameState == GameState::EXIT) 
	{
		//Free resources and close SDL
		close();
	}
}

void MainGame::processInput(SDL_Rect camera)
{
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			gameState = GameState::EXIT;
		}

		//Handle input for the player
		player.handleEvent(e, camera);
	}
}

void MainGame::playGame()
{
	//Move player
	player.move();
	player.setCamera(_camera);

	//Update enemies
	for (int i = 0; i < enemyVec.size();)
	{
		if (enemyVec[i].update(player.getX(), player.getY(), player.getBox()))
		{
			//Little trick to easily delete element from vector; swap with the last element of the vector and remove the last element
			enemyVec[i] = enemyVec.back();
			enemyVec.pop_back();
		}
		//Only increment i if an enemy wasn't deleted (if i++ is inside for(), skips over the enemy that is in enemyVec.back())
		else
		{
			i++;
		}
	}

	//Check if no enemies are left
	if (enemyVec.empty())
	{
		//Victory!
		victory = 1;
		gameState = GameState::OVER;
	}
	
	//Update bullets
	for (int i = 0; i < bulletVec.size();)
	{
		if (bulletVec[i].update())
		{
			//Little trick to easily delete element from vector; swap with the last element of the vector and remove the last element
			bulletVec[i] = bulletVec.back();
			bulletVec.pop_back();
		}
		//Only increment i if a bullet wasn't deleted (if i++ is inside for(), skips over the bullet that is in bulletVec.back())
		else
		{
			i++;
		}
	}

	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	//Render level
	level.render(_camera);

	//Render player
	player.render(_camera);

	//Render enemies
	for (int i = 0; i < enemyVec.size(); i++)
	{
		enemyVec[i].render(_camera);
	}

	//Render bullets
	for (int i = 0; i < bulletVec.size(); i++)
	{
		bulletVec[i].render(_camera);
	}

	//Update screen
	SDL_RenderPresent(renderer);
}

bool MainGame::gameOver()
{
	if (victory)
	{
		std::cout << "Victory!\n";
	}
	else
	{ 
		std::cout << "Defeat!\n";
	}

	std::cout << "Restart? (1/0)\n";
	int vastaus;
	std::cin >> vastaus;
	if (vastaus == 1) return true;
	else return false;
}

void MainGame::restart()
{
	gameState = GameState::PLAY;
	enemyVec.clear();
	level.setTiles();
	player.restart();
	gameLoop();
}
